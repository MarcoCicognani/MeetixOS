/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
*         This software is derived from the Ghost operating system project,           *
*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
*         https://ghostkernel.org/                                                    *
*         https://github.com/maxdev1/ghost                                            *
*                                                                                     *
* This program is free software; you can redistribute it and/or                       *
* modify it under the terms of the GNU General Public License                         *
* as published by the Free Software Foundation; either version 2                      *
* of the License, or (char *argumentat your option) any later version.                *
*                                                                                     *
* This program is distributed in the hope that it will be useful,                     *
* but WITHout ANY WARRANTY; without even the implied warranty of                      *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
* GNU General Public License for more details.                                        *
*                                                                                     *
* You should have received a copy of the GNU General Public License                   *
* along with this program; if not, write to the Free Software                         *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "filesystem/filesystem.hpp"
#include "filesystem/FsDescriptors.hpp"
#include "filesystem/FsTransactionStore.hpp"
#include "filesystem/FsDelegate.hpp"
#include "filesystem/FsDelegateMount.hpp"
#include "filesystem/FsDelegateRamdisk.hpp"
#include "filesystem/FsDelegatePipe.hpp"
#include "filesystem/FsDelegateTasked.hpp"
#include "filesystem/pipes.hpp"

#include "logger/logger.hpp"

#include "eva/utils/local.hpp"
#include "utils/HashMap.hpp"
#include "utils/ListEntry.hpp"
#include "utils/string.hpp"

/**
 *
 */
static FsVirtID nodeNextID = 0;
static HashMap<FsVirtID, FsNode*> *nodes;

static FsNode *root;
static FsNode *pipeRoot;
static FsNode *mountRoot;

/**
 *
 */
void FileSystem::initialize()
{
	Pipes::initialize();
	FileDescriptors::initialize();
	FsTransactionStore::initialize();
	nodes = new HashMap<FsVirtID, FsNode*>();

	// create root
	root = createNode();
	root->type = FS_NODE_TYPE_ROOT;

	// mount root
	mountRoot = createNode();
	mountRoot->setDelegate(new FsDelegateMount());
	mountRoot->name = (char*) "mount";
	mountRoot->type = FS_NODE_TYPE_MOUNTPOINT;
	root->addChild(mountRoot);
	DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(mountRoot);

	// ramdisk root
	FsNode *ramdiskRoot = createNode();
	FsDelegate *ramdiskDelegate = new FsDelegateRamdisk();
	ramdiskRoot->setDelegate(ramdiskDelegate);
	ramdiskRoot->name = (char*) "ramdisk";
	ramdiskRoot->type = FS_NODE_TYPE_MOUNTPOINT;
	mountRoot->addChild(ramdiskRoot);
	DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(ramdiskRoot);

	// pipe root
	pipeRoot = createNode();
	pipeRoot->setDelegate(new FsDelegatePipe());
	pipeRoot->name = (char*) "pipe";
	pipeRoot->type = FS_NODE_TYPE_MOUNTPOINT;
	mountRoot->addChild(pipeRoot);
	DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(pipeRoot);

	// ramdisk is root
	root->setDelegate(ramdiskDelegate);

	logInfo("%! initial resources created", "filesystem");
}

/**
 *
 */
FsNode *FileSystem::getRoot()
{
	return root;
}

/**
 *
 */
FsNode *FileSystem::getNodeById(FsVirtID id)
{
	auto entry = nodes->get(id);
	if (entry) return entry->value;
	return 0;
}

/**
 *
 */
FsNode *FileSystem::createNode()
{
	FsNode *node = new FsNode();
	node->id = nodeNextID++;
	nodes->add(node->id, node);
	return node;
}

/**
 *
 */
void FileSystem::findExisting(char *absolutePath, FsNode **outParent, FsNode **outChild, char *nameCurrent, bool followSymlinks)
{
	FsNode *parent = 0;
	FsNode *child = root;

	char *abspos = absolutePath;

	while (true)
	{
		// parent is now child
		parent = child;

		// copy first name on left into current
		while (*abspos == '/')
			++abspos;

		char *curpos = nameCurrent;
		int curlen = 0;
		while (*abspos != 0 && *abspos != '/')
		{
			*curpos++ = *abspos++;
			++curlen;
		}
		*curpos = 0;

		// quit if nothing left in path
		if (curlen == 0) break;

		// handle specials
		if (String::equals("..", nameCurrent))
		{
			if (parent->parent != 0) child = parent->parent;
		}
		else if (String::equals(".", nameCurrent)) { /* skip */ }
		else
		{
			// check if requested child exists
			child = parent->findChild(nameCurrent);

			if (child == 0) break;
		}
	}

	*outParent = parent;
	*outChild = child;
}

/**
 *
 */
void FileSystem::processClosed(Pid pid)
{
	FileDescriptorTable *table = FileDescriptors::getProcessTable(pid);

	// close each entry
	for (auto iter = table->descriptors.begin(); iter != table->descriptors.end(); ++iter)
	{
		FileDescriptorContent *content = iter->value;

		auto nodeEntry = nodes->get(content->nodeID);
		if (nodeEntry)
		{
			FsCloseStatus stat = unmapFile(pid, nodeEntry->value, content);

			if (stat == FS_CLOSE_SUCCESSFUL)
			{
				logDebug("%! successfully closed fd %i when exiting process %i", "filesystem", content->id, pid);
			}

			else
			{
				logDebug("%! failed to close fd %i when exiting process %i with status %i", "filesystem", content->id, pid, stat);
			}
		}
	}

	// remove all entries
	FileDescriptors::unmapAll(pid);
}

/**
 *
 */
void FileSystem::processForked(Pid source, Pid fork)
{
	FileDescriptorTable *sourceTable = FileDescriptors::getProcessTable(source);

	// clone each entry
	for (auto iter = sourceTable->descriptors.begin(); iter != sourceTable->descriptors.end(); ++iter)
	{
		FileDescriptorContent *content = iter->value;

		FsClonefdStatus stat;
		clonefd(content->id, source, content->id, fork, &stat);
		logDebug("%! forking cloned fd %i from process %i -> %i with status %i", "filesystem", content->id, source, fork, stat);
	}
}

/**
 *
 */
File_t FileSystem::mapFile(Pid pid, FsNode *node, int32_t openFlags, File_t fd)
{
	if (node->type == FS_NODE_TYPE_FILE) return FileDescriptors::map(pid, node->id, fd, openFlags);

	else if (node->type == FS_NODE_TYPE_PIPE)
	{
		Pipes::addReference(node->physFsID, pid);
		return FileDescriptors::map(pid, node->id, fd, openFlags);
	}

	logWarn("%! tried to open a node of non-file type %i", "filesystem", node->type);
	return -1;
}

/**
 *
 */
bool FileSystem::unmapFile(Pid pid, FsNode *node, FileDescriptorContent *fd)
{
	if (node->type == FS_NODE_TYPE_FILE)
	{
		FileDescriptors::unmap(pid, fd->id);
		return true;
	}

	else if (node->type == FS_NODE_TYPE_PIPE)
	{
		Pipes::removeReference(node->physFsID, pid);
		FileDescriptors::unmap(pid, fd->id);
		return true;
	}

	logWarn("%! tried to close a node of non-file type %i", "filesystem", node->type);
	return false;
}

/**
 *
 */
void FileSystem::getRealPathToNode(FsNode *node, char *out)
{
	FsNode *current = node;

	int absLen = 0;
	while (current != 0)
	{

		// on root we can stop
		if (current->type == FS_NODE_TYPE_ROOT) break;

		// check
		if (current->name == 0)
		{
			logWarn("%! problem: tried to add name of nameless node %i", "filesystem", current->id);
			break;
		}

		// get & check length + slash
		int nameLen = String::length(current->name);
		if (absLen + nameLen + 1 > PATH_MAX)
		{
			logWarn("%! problem: tried to create a path thats longer than PATH_MAX from a node", "filesystem");
			break;
		}

		// copy so that out contains: current->name "/" out
		for (int i = absLen; i >= 0; i--)
			out[i + nameLen + 1] = out[i];

		out[nameLen] = '/';
		Memory::copy(out, current->name, nameLen);

		absLen += nameLen + 1;

		current = current->parent;
	}

	// add final slash & null termination
	for (int i = absLen; i >= 0; i--)
		out[i + 1] = out[i];

	out[0] = '/';
	out[absLen + 1] = 0;
}

/**
 *
 */
void FileSystem::concatAsAbsolutePath(char *relativeBase, char *in, char *out)
{
	// check if valid input
	int lenIn = String::length(in);
	if (lenIn == 0)
	{
		out[0] = 0;
		return;
	}

	// if path is absolute, only copy to out
	if (in[0] == '/') String::copy(out, in);
	else
	{
		// otherwise append: cwd + "/" + in
		int lenCwd = String::length(relativeBase);
		int lenSep = 1;
		Memory::copy(out, relativeBase, lenCwd);
		Memory::copy(&out[lenCwd], "/", lenSep);
		Memory::copy(&out[lenCwd + lenSep], in, lenIn);
		out[lenCwd + lenIn + lenSep] = 0;
	}
}

/**
 *
 */
FsRegisterAsDelegateStatus FileSystem::createDelegate(Thread *thread, char *name, FsPhysID physMountpointID, FsVirtID *outMountpointID, Address *outTransactionStorage)
{
	FsNode *existing = root->findChild(name);
	if (existing) return FS_REGISTER_AS_DELEGATE_FAILED_EXISTING;

	// create and prepare delegate
	FsDelegateTasked *taskedDelegate = new FsDelegateTasked(thread);
	if (!taskedDelegate->prepare(outTransactionStorage))
	{
		delete taskedDelegate;
		logInfo("%! failed to create delegate", "filesystem");

		return FS_REGISTER_AS_DELEGATE_FAILED_DELEGATE_CREATION;
	}

	// create mountpoint
	FsNode *mountpoint = createNode();
	mountpoint->setDelegate(taskedDelegate);
	mountpoint->name = new char[String::length(name) + 1];
	String::copy(mountpoint->name, name);

	mountpoint->type = FS_NODE_TYPE_MOUNTPOINT;
	mountpoint->physFsID = physMountpointID;
	mountRoot->addChild(mountpoint);

	DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(mountpoint);

	// copy mountpoint id
	*outMountpointID = mountpoint->id;

	logInfo("%! mountpoint '%s' (node id %i) is handled by delegate task %i", "filesystem", mountpoint->name, mountpoint->id, thread->id);

	return FS_REGISTER_AS_DELEGATE_SUCCESSFUL;
}

/**
 *
 */
bool FileSystem::nodeForDescriptor(Pid pid, File_t fd, FsNode **outNode, FileDescriptorContent  **outFd)
{
	// find file descriptor
	FileDescriptorContent *fdContent = FileDescriptors::get(pid, fd);
	if (fdContent == 0) return false;

	// get node for id in fd
	FsVirtID nodeid = fdContent->nodeID;
	auto entry = nodes->get(nodeid);
	if (entry == 0) return false;

	*outFd = fdContent;
	*outNode = entry->value;

	return true;
}

/**
 *
 */
File_t FileSystem::clonefd(File_t sourceFd, Pid sourcePid, File_t targetFd, Pid targetPid, FsClonefdStatus *outStatus)
{
	FsNode *sourceNode;
	FileDescriptorContent *sourceFdContent;
	if (!nodeForDescriptor(sourcePid, sourceFd, &sourceNode, &sourceFdContent))
	{
		*outStatus = FS_CLONEFD_INVALID_SOURCE_FD;
		return -1;
	}

	FileDescriptorContent *targetFdContent = 0;
	FsNode *targetNode = 0;
	if (targetFd != -1)
	{
		nodeForDescriptor(targetPid, targetFd, &targetNode, &targetFdContent);

		// close old file descriptor if available
		if (targetNode) unmapFile(sourcePid, targetNode, targetFdContent);
	}

	// open new file descriptor
	File_t created = mapFile(targetPid, sourceNode, 0, targetFd);
	if (created != -1)
	{
		// clone fd contents
		FileDescriptorContent *createdFdContent = 0;
		FsNode *createdNode = 0;
		if (!nodeForDescriptor(targetPid, created, &createdNode, &createdFdContent))
		{
			*outStatus = FS_CLONEFD_ERROR;
			return -1;
		}
		sourceFdContent->cloneInto(createdFdContent);

		// operation fine
		*outStatus = FS_CLONEFD_SUCCESSFUL;
		return created;
	}

	*outStatus = FS_CLONEFD_ERROR;
	return -1;
}

FsPipeStatus FileSystem::pipe(Thread *thread, File_t *outWrite, File_t *outRead)
{
	FsNode *node = createNode();
	node->type = FS_NODE_TYPE_PIPE;
	pipeRoot->addChild(node);

	node->physFsID = Pipes::create();
	*outWrite = mapFile(thread->process->main->id, node, 0);
	*outRead = mapFile(thread->process->main->id, node, 0);
	return FS_PIPE_SUCCESSFUL;
}
