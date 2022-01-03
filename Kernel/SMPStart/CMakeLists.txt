set(ASM_SOURCES smp.asm)
set_source_files_properties(${ASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

set(CMAKE_ASM_NASM_COMPILE_OBJECT "${CMAKE_ASM_NASM_COMPILER} -f bin -o <OBJECT> -s <SOURCE>")
add_executable(SMPStart.ko ${ASM_SOURCES})
add_custom_command(
        TARGET SMPStart.ko
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:SMPStart.ko> ${CMAKE_CURRENT_BINARY_DIR}/SMPStart.ko
)
install(TARGETS SMPStart.ko DESTINATION MeetiX/Kernel/Modules)
