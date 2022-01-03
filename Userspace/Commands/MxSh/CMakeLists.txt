add_executable(MxSh environment.cpp interpreter.cpp mx.cpp mxscript.cpp parser.cpp shell.cpp)
target_link_libraries(MxSh LibApi LibC LibUtils LibIO LibTasking)
install(TARGETS MxSh DESTINATION Bins)
