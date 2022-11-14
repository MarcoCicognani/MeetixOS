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
        system/interrupts/descriptors/IdtMounter.asm
        system/interrupts/handling/InterruptStubs.asm
        system/processor.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

set(CCLANG_SOURCES
        ../../CCLang/Alloc/Details.cc
        ../../CCLang/Alloc/RefCounted.cc
        ../../CCLang/Alloc/String.cc
        ../../CCLang/Alloc/StringBuilder.cc
        ../../CCLang/Alloc/StringStorage.cc
        ../../CCLang/Alloc/Text/Format.cc
        ../../CCLang/Alloc/Text/FormatLexer.cc
        ../../CCLang/Alloc/Text/FormatParser.cc
        ../../CCLang/Alloc/Text/Formatter.cc
        ../../CCLang/Alloc/Text/Lexer.cc
        ../../CCLang/Core/Error.cc
        ../../CCLang/Core/Hashing.cc
        ../../CCLang/Core/SourceLocation.cc
        ../../CCLang/Lang/StringView.cc)

set(LIBC_SOURCES
        ../../Userspace/Libraries/LibC/errno.cc
        ../../Userspace/Libraries/LibC/string.cc)

set(KERNEL_SOURCES
        calls/SyscallHandler.cpp
        calls/SyscallHandlerFilesystem.cpp
        calls/SyscallHandlerInfo.cpp
        calls/SyscallHandlerLog.cpp
        calls/SyscallHandlerMemory.cpp
        calls/SyscallHandlerMessaging.cpp
        calls/SyscallHandlerServer.cpp
        calls/SyscallHandlerSpawning.cpp
        calls/SyscallHandlerTasking.cpp
        calls/SyscallHandlerTime.cc
        calls/SyscallHandlerVm86.cpp
        debug/DebugInterfaceKernel.cpp
        EvangelionNG.cpp
        executable/Elf32Loader.cpp
        filesystem/filesystem.cpp
        filesystem/FsDelegateMount.cpp
        filesystem/FsDelegatePipe.cpp
        filesystem/FsDelegateRamdisk.cpp
        filesystem/FsDelegateTasked.cpp
        filesystem/FsDescriptors.cpp
        filesystem/FsNode.cpp
        filesystem/FsTransactionHandlerClose.cpp
        filesystem/FsTransactionHandlerDirectoryRefresh.cpp
        filesystem/FsTransactionHandlerDiscovery.cpp
        filesystem/FsTransactionHandlerGetLength.cpp
        filesystem/FsTransactionHandlerOpen.cpp
        filesystem/FsTransactionHandlerRead.cpp
        filesystem/FsTransactionHandlerReadDirectory.cpp
        filesystem/FsTransactionHandlerWrite.cpp
        filesystem/FsTransactionStore.cpp
        filesystem/pipes.cpp
        init.cpp
        logger/logger.cpp
        memory/AddressSpace.cpp
        memory/allocators/ChunkAllocator.cpp
        memory/collections/AddressRangePool.cpp
        memory/collections/AddressStack.cpp
        memory/gdt/GdtManager.cpp
        memory/KernelHeap.cpp
        memory/LowerHeap.cpp
        memory/physical/PPallocator.cpp
        memory/physical/PPreferenceTracker.cpp
        memory/TemporaryPagingUtil.cpp
        ramdisk/Ramdisk.cpp
        system/acpi/acpi.cpp
        system/acpi/AcpiEntry.cpp
        system/acpi/madt.cpp
        system/acpi/RsdpLookupUtil.cpp
        system/interrupts/descriptors/idt.cpp
        system/interrupts/descriptors/ivt.cpp
        system/interrupts/handling/InterruptDispatcher.cpp
        system/interrupts/handling/InterruptExceptionHandler.cpp
        system/interrupts/handling/InterruptRequestHandler.cpp
        system/interrupts/ioapic.cpp
        system/interrupts/IoapicManager.cpp
        system/interrupts/lapic.cpp
        system/interrupts/pic.cpp
        system/pci/pci.cpp
        system/processor.cpp
        system/smp/GlobalLock.cpp
        system/smp/GlobalRecursiveLock.cpp
        system/smp/smp.cpp
        system/system.cpp
        system/timing/pit.cpp
        system/timing/RTC.cc
        tasking/communication/MessageController.cpp
        tasking/process.cpp
        tasking/scheduling/scheduler.cpp
        tasking/tasking.cpp
        tasking/thread.cpp
        tasking/ThreadManager.cpp
        utils/string.cpp
        vm86/Virtual8086Monitor.cpp)

add_executable(Kernel
        ${ASM_SOURCES}
        ${LIBC_SOURCES}
        ${CCLANG_SOURCES}
        ${KERNEL_SOURCES})

target_include_directories(Kernel PRIVATE . ../Shared)
target_link_libraries(Kernel Shared gcc)
target_link_options(Kernel PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/LinkScript.ld -nostdlib -nostartfiles)

install(TARGETS Kernel DESTINATION ../ISO/boot)