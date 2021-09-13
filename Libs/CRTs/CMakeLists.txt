add_library(crt0 STATIC crt0.S)
add_custom_command(
        TARGET crt0
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crt0> ${TOOLCHAIN_ROOT}/lib/crt0.o
)

add_library(crti STATIC crti.S)
add_custom_command(
        TARGET crti
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crti> ${TOOLCHAIN_ROOT}/lib/crti.o
)

add_library(crtn STATIC crtn.S)
add_custom_command(
        TARGET crtn
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:crtn> ${TOOLCHAIN_ROOT}/lib/crtn.o
)
