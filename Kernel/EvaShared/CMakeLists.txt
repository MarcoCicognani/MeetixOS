set(ASM_SOURCES memory/gdt/GdtMounter.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

add_library(EvaShared
            STATIC
            debug/DebugInterface.cpp
            logger/logger.cpp
            memory/bitmap/BitmapPageAllocator.cpp
            memory/gdt/gdt.cpp
            ${ASM_SOURCES}
            memory/memory.cpp
            multiboot/MultibootUtil.cpp
            runtime/constructors.cpp
            runtime/ItaniumCXXAbiSupport.cpp
            system/serial/SerialPort.cpp
            system/BiosDataArea.cpp
            system/IOPorts.cpp
            utils/string.cpp
            video/ConsoleVideo.cpp
            video/PrettyBoot.cpp)
target_include_directories(EvaShared PRIVATE inc .)