set(ASM_SOURCES loader.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

add_executable(EvaLoader
        memory/MultibootMmapInterpreter.cpp
        memory/PagingInitializer.cpp
        memory/gdt/GdtManager.cpp
        ${ASM_SOURCES}
        init.cpp
        utils/string.cpp
        loader.cpp
        logger/logger.cpp
        multiboot/MultibootModuleAnalyzer.cpp
        kernelloader/KernelLoader.cpp)

target_include_directories(EvaLoader PRIVATE . ../EvaShared)
target_link_libraries(EvaLoader EvaShared)
target_link_options(EvaLoader PRIVATE LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/../extra/link-loader.ld -nostdlib -nostartfiles)