/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include "spawner.hpp"

#include "Elf32Loader.hpp"
#include "Power.hpp"

#include <Api/ByteWise.h>
#include <fcntl.h>
#include <io/files/futils.hpp>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <utils/arguments.hpp>
#include <utils/environment.hpp>
#include <utils/utils.hpp>
#include <vector>

/**
 *
 */
int main(int argc, char* argv[]) {
    // register this task as the system spawner
    if ( !s_register_as_server(SPAWNER_IDENTIFIER, SECURITY_LEVEL_KERNEL) ) {
        klog("failed to initialize spawner service: could not register with "
             "identifier %s",
             SPAWNER_IDENTIFIER);
        return -1;
    }

    // initialize the system userspace
    init();

    // main loop of spawner
    receiveRequests();
}

/**
 *	create environment file
 */
void createOsEnvironmentFile(const char* dir, const char* fileName) {
    // open directory etc
    s_set_working_directory(dir);

    // define first variables
    const char base_env[]
        = { "HOSTNAME=MeetiX_OS\nVERSION=0.7.1\nSYSTEM_LEVEL=basic\nPATH=/Bins/:/Apps/"
            ":/MeetiX/Kernel/Servers/\nTHEME=Green" };
    auto base_env_len = strlen(base_env);

    // create the environment file
    auto env_fd = s_open_f(fileName, O_READ | O_WRITE);
    if ( env_fd == FD_NONE ) {
        env_fd = s_open_f(fileName, O_CREAT | O_READ | O_WRITE);

        // write variables on file and close it
        s_write(env_fd, base_env, base_env_len);
    }

    s_close(env_fd);
}

/**
 *	create s_log file
 */
void createOsLogFile(const char* dir, const char* fileName) {
    // open directory etc
    s_set_working_directory(dir);

    const char loginput[] = { "[0] s_log file created\n" };
    int        length     = strlen(loginput);

    // create the log file
    // File_t logfile = OpenF(fileName, O_CREAT | O_READ | O_WRITE);

    // write variables on file and close it
    // Write(logfile, loginput, length);
    // Close(logfile);
}

void init() {
    // let the spawner load shell
    Pid         sh_pid;
    FileHandle  out_stdio;
    SpawnStatus stat = spawn("/Bins/MxSh",
                             "-s Start.sh",
                             "/MeetiX/Configs/Startup/Minimal",
                             SECURITY_LEVEL_KERNEL,
                             0,
                             &sh_pid,
                             &out_stdio,
                             &out_stdio,
                             &out_stdio,
                             FD_NONE,
                             FD_NONE,
                             FD_NONE);

    if ( stat == SPAWN_STATUS_SUCCESSFUL ) {
        klog("MxSh executed in process %d", sh_pid);
        klog("Creating environment variables file");
        createOsEnvironmentFile("/MeetiX/Configs/Env/", "Global");

        klog("Creating log file");
        createOsLogFile("/MeetiX/", "log");
    } else
        klog("failed to load mx shell from '/Bins/MxSh' with code %d", stat);
}

/**
 *
 */
[[noreturn]] void receiveRequests() {
    // service ready
    klog("spawner server ready");

    // defining size for messages
    const size_t requestLenMax = sizeof(MessageHeader) + sizeof(SpawnCommandSpawnRequest) + 1024;
    while ( true ) {
        // creating buffer for message
        uint8_t requestBuffer[requestLenMax];

        // receive incoming request
        auto stat = s_receive_message(requestBuffer, requestLenMax);
        if ( stat != MESSAGE_RECEIVE_STATUS_SUCCESSFUL )
            protocolError("receiving command failed with code %i", stat);

        auto header        = (MessageHeader*)requestBuffer;
        auto commandHeader = (SpawnCommandHeader*)MESSAGE_CONTENT(header);

        if ( commandHeader->m_command == SPAWN_COMMAND_SPAWN_REQUEST )
            processSpawnRequest((SpawnCommandSpawnRequest*)commandHeader,
                                header->m_sender_tid,
                                header->m_transaction);
        else if ( commandHeader->m_command == SPAWN_COMMAND_SHUTDOWN_MACHINE
                  || commandHeader->m_command == SPAWN_COMMAND_REBOOT_MACHINE )
            processHaltMachine(commandHeader->m_command);
        else
            protocolError("received unknown command: code %i, task %i",
                          commandHeader->m_command,
                          header->m_sender_tid);
    }
}

/**
 *
 */
void protocolError(std::string msg, ...) {
    va_list va;
    va_start(va, msg);
    kvlog(("protocol error: " + msg).c_str(), va);
    va_end(va);
}

/**
 *
 */
void processSpawnRequest(SpawnCommandSpawnRequest* request, Tid requester, MessageTransaction tx) {
    SecurityLevel secLvl = request->m_security_level;

    auto pos = (const char*)request;
    pos      = pos + sizeof(SpawnCommandSpawnRequest);

    auto path = pos;
    pos       = pos + request->m_path_len;

    auto args = pos;
    pos       = pos + request->m_args_len;

    auto workdir = pos;

    // parameters ready, perform spawn
    Pid         oPid;
    FileHandle  oFdInw;
    FileHandle  oFdOutr;
    FileHandle  oFdErr;
    Pid         requesterPid = s_get_pid_for_tid(requester);
    SpawnStatus spawnStatus  = spawn(path,
                                     args,
                                     workdir,
                                     secLvl,
                                     requesterPid,
                                     &oPid,
                                     &oFdInw,
                                     &oFdOutr,
                                     &oFdErr,
                                     request->m_stdin,
                                     request->m_stdout,
                                     request->m_stderr);

    // send response
    SpawnCommandSpawnResponse response;
    response.m_new_process_id = oPid;
    response.m_spawn_status   = spawnStatus;
    response.m_stdin_write    = oFdInw;
    response.m_stdout_read    = oFdOutr;
    response.m_stderr_read    = oFdErr;
    s_send_message_t(requester, &response, sizeof(SpawnCommandSpawnResponse), tx);
}

/**
 *
 */
bool createPipe(Pid         thisPid,
                Pid         requesterPid,
                Pid         targetPid,
                FileHandle  source,
                FileHandle* out,
                FileHandle  target) {
    FileHandle created = FD_NONE;
    if ( source == FD_NONE ) {
        // create pipe
        FileHandle   pipe[2];
        FsPipeStatus pipeStat;
        s_pipe_s(&pipe[0], &pipe[1], &pipeStat);

        if ( pipeStat == FS_PIPE_SUCCESSFUL ) {
            // map into target & requester
            created = s_clone_fd_t(pipe[1], thisPid, target, targetPid);
            *out    = s_clone_fd(pipe[0], thisPid, requesterPid);

            // close pipe here
            s_close(pipe[0]);
            s_close(pipe[1]);
        }
    } else {
        // map into target
        created = s_clone_fd_t(source, requesterPid, target, targetPid);
        *out    = source;
    }

    return created != FD_NONE;
}

/**
 *
 */
bool setupStdio(Pid         createdPid,
                Pid         requesterPid,
                FileHandle* outStdin,
                FileHandle* outStdout,
                FileHandle* outStderr,
                FileHandle  inStdin,
                FileHandle  inStdout,
                FileHandle  inStderr) {
    auto thisPid = s_get_pid();
    if ( !createPipe(thisPid, requesterPid, createdPid, inStdin, outStdin, STDIN_FILENO) )
        return false;
    if ( !createPipe(thisPid, requesterPid, createdPid, inStdout, outStdout, STDOUT_FILENO) )
        return false;
    if ( !createPipe(thisPid, requesterPid, createdPid, inStderr, outStderr, STDERR_FILENO) )
        return false;

    return true;
}

/**
 *
 */
void writeCliArgs(ProcessCreationIdentifier targetProc, const char* args) {
    // prepare well-sized buffer
    char*  argsBuf = new char[CLIARGS_BUFFER_LENGTH];
    size_t argsLen = strlen(args);

    // copy arguments to buffer
    memcpy(argsBuf, args, argsLen);
    argsBuf[argsLen] = 0;

    // store arguments
    s_cli_args_store(targetProc, argsBuf);

    // free buffer
    delete[] argsBuf;
}

/**
 *
 */
BinaryFormat detectFormat(FileHandle file) {
    // try to detect the format
    BinaryFormat form = BF_UNKNOWN;
    if ( Elf32Loader::checkForElfBinaryAndReset(file) == LS_SUCCESSFUL )
        form = BF_ELF32;

    return form;
}

/*
 *
 */
bool findFile(const char* path, FileHandle* file) {
    // try open with provided path
    if ( (*file = s_open_f(path, O_RDONLY)) != FD_NONE )
        return true;

    // try adding the path
    auto paths = Arguments::split(Environment::get("PATH"), ':');
    for ( auto& dir : paths ) {
        // app directory is composited
        if ( dir == "Apps" )
            dir += std::string{ path } + std::string{ "/Bin" };

        // try open the file
        if ( (*file = s_open_f((dir + std::string(path)).c_str(), O_RDONLY)) != FD_NONE )
            return true;
    }

    return false;
}

/**
 *
 */
SpawnStatus spawn(const char*   path,
                  const char*   args,
                  const char*   workdir,
                  SecurityLevel secLvl,
                  Pid           requesterPid,
                  Pid*          outPid,
                  FileHandle*   outStdin,
                  FileHandle*   outStdout,
                  FileHandle*   outStderr,
                  FileHandle    inStdin,
                  FileHandle    inStdout,
                  FileHandle    inStderr) {
    // file of executable
    FileHandle file;

    // open input file
    if ( !findFile(path, &file) ) {
        klog("unable to open the file: %s, it doesn't exist on PATH directory", path);
        return SPAWN_STATUS_IO_ERROR;
    }

    // detect format
    BinaryFormat format = detectFormat(file);
    if ( format == BF_UNKNOWN ) {
        klog("binary has an unknown format: %s", path);
        return SPAWN_STATUS_FORMAT_ERROR;
    }

    // create empty target process
    auto targetProc = s_create_empty_process(secLvl);
    Pid  targetPid  = s_get_created_process_id(targetProc);

    // apply configuration
    ProcessConfiguration configuration;
    configuration.m_source_path = (char*)path;
    s_configure_process(targetProc, configuration);

    // create a loader
    Loader* loader;
    if ( format == BF_ELF32 )
        loader = new Elf32Loader(targetProc, file);
    // else if (format == BF_ELF64) loader = new Elf64Loader(targetProc, file);
    else {
        klog("no loader implementation for binary format %i", format);
        return SPAWN_STATUS_FORMAT_ERROR;
    }

    // setup standard I/O
    if ( !setupStdio(targetPid,
                     requesterPid,
                     outStdin,
                     outStdout,
                     outStderr,
                     inStdin,
                     inStdout,
                     inStderr) )
        klog("unable to setup stdio for process %i", targetPid);

    // perform loading
    uintptr_t    entryAddress;
    LoaderStatus ldrStat   = loader->load(&entryAddress);
    SpawnStatus  spawnStat = SPAWN_STATUS_UNKNOWN;

    if ( ldrStat == LS_SUCCESSFUL ) {
        // push command line arguments
        writeCliArgs(targetProc, args);

        // set working directory
        s_set_working_directory_p(workdir, targetProc);

        // attached loaded process
        s_attach_created_process(targetProc, entryAddress);

        // out-set process id
        *outPid   = targetPid;
        spawnStat = SPAWN_STATUS_SUCCESSFUL;
    } else {
        // cancel creation & let kernel clean up
        s_cancel_process_creation(targetProc);

        if ( ldrStat == LS_IO_ERROR )
            spawnStat = SPAWN_STATUS_IO_ERROR;
        else if ( ldrStat == LS_FORMAT_ERROR )
            spawnStat = SPAWN_STATUS_FORMAT_ERROR;
        else if ( ldrStat == LS_MEMORY_ERROR )
            spawnStat = SPAWN_STATUS_MEMORY_ERROR;
        else {
            klog("loader return unknown status on failure %i", ldrStat);
            spawnStat = SPAWN_STATUS_UNKNOWN;
        }
    }

    // Close binary file
    s_close(file);
    return spawnStat;
}
