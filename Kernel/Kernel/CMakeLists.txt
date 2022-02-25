#
# @brief
# This file is part of the MeetiX Operating System.
# Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
#
# @developers
# Marco Cicognani (marco.cicognani@meetixos.org)
#
# @license
# GNU General Public License version 3
#

set(ASM_SOURCES
        system/interrupts/handling/InterruptStubs.asm
        system/interrupts/descriptors/IdtMounter.asm
        system/processor.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

# TODO missing ctype, memcpy, memset, memmove, strlen
set(LIBTC_SOURCES
        ../../Libs/LibTC/Collection/String.cc
        ../../Libs/LibTC/Collection/StringBuilder.cc
        ../../Libs/LibTC/Collection/StringStorage.cc
        ../../Libs/LibTC/Collection/StringView.cc
        ../../Libs/LibTC/Hashing/Integer.cc
        ../../Libs/LibTC/Hashing/Pointer.cc
        ../../Libs/LibTC/Memory/Shareable.cc
        ../../Libs/LibTC/Text/Formatter.cc
        ../../Libs/LibTC/Text/Lexer.cc)

set(LIBC_SOURCES
        ../../Libs/LibC/ctype.cc
        ../../Libs/LibC/string.cc)

add_executable(Kernel
        EvangelionNG.cpp
        filesystem/FsDelegateTasked.cpp
        filesystem/FsDelegatePipe.cpp
        filesystem/FsDelegateRamdisk.cpp
        filesystem/FsTransactionHandlerGetLength.cpp
        filesystem/FsTransactionHandlerWrite.cpp
        filesystem/FsTransactionHandlerDiscovery.cpp
        filesystem/FsTransactionHandlerClose.cpp
        filesystem/FsTransactionHandlerReadDirectory.cpp
        filesystem/FsTransactionHandlerOpen.cpp
        filesystem/pipes.cpp
        filesystem/FsTransactionStore.cpp
        filesystem/FsDescriptors.cpp
        filesystem/FsTransactionHandlerRead.cpp
        filesystem/FsTransactionHandlerDirectoryRefresh.cpp
        filesystem/FsNode.cpp
        filesystem/FsDelegateMount.cpp
        filesystem/filesystem.cpp
        memory/LowerHeap.cpp
        memory/collections/AddressStack.cpp
        memory/collections/AddressRangePool.cpp
        memory/physical/PPallocator.cpp
        memory/physical/PPreferenceTracker.cpp
        memory/AddressSpace.cpp
        memory/gdt/GdtManager.cpp
        memory/allocators/ChunkAllocator.cpp
        memory/TemporaryPagingUtil.cpp
        memory/KernelHeap.cpp
        init.cpp
        ramdisk/Ramdisk.cpp
        calls/SyscallHandlerVm86.cpp
        calls/SyscallHandlerMemory.cpp
        calls/SyscallHandlerInfo.cpp
        calls/SyscallHandlerLog.cpp
        calls/SyscallHandler.cpp
        calls/SyscallHandlerMessaging.cpp
        calls/SyscallHandlerFilesystem.cpp
        calls/SyscallHandlerSpawning.cpp
        calls/SyscallHandlerTasking.cpp
        calls/SyscallHandlerServer.cpp
        calls/SyscallHandlerTime.cc
        utils/string.cpp
        logger/logger.cpp
        tasking/tasking.cpp
        tasking/thread.cpp
        tasking/process.cpp
        tasking/ThreadManager.cpp
        tasking/communication/MessageController.cpp
        tasking/scheduling/scheduler.cpp
        debug/DebugInterfaceKernel.cpp
        executable/Elf32Loader.cpp
        system/interrupts/handling/InterruptExceptionHandler.cpp
        system/interrupts/handling/InterruptDispatcher.cpp
        system/interrupts/handling/InterruptRequestHandler.cpp
        system/interrupts/ioapic.cpp
        system/interrupts/lapic.cpp
        system/interrupts/IoapicManager.cpp
        system/interrupts/descriptors/ivt.cpp
        system/interrupts/descriptors/idt.cpp
        system/interrupts/pic.cpp
        system/processor.cpp
        system/system.cpp
        system/smp/smp.cpp
        system/smp/GlobalLock.cpp
        system/smp/GlobalRecursiveLock.cpp
        system/pci/pci.cpp
        system/timing/pit.cpp
        system/acpi/RsdpLookupUtil.cpp
        system/acpi/acpi.cpp
        system/acpi/AcpiEntry.cpp
        system/acpi/madt.cpp
        system/timing/RTC.cc
        vm86/Virtual8086Monitor.cpp
        ${ASM_SOURCES}
        ${LIBTC_SOURCES}
        ${LIBC_SOURCES})

target_include_directories(Kernel PRIVATE . ../Shared)
target_link_libraries(Kernel Shared)
target_link_options(Kernel PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/LinkScript.ld -nostdlib -nostartfiles)

install(TARGETS Kernel DESTINATION ../ISO/boot)