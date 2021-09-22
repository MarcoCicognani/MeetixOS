add_executable(Time.sv time.cpp)
target_include_directories(Time.sv PRIVATE .)
target_link_libraries(Time.sv LibApi LibC LibIO LibUtils)
install(TARGETS Time.sv DESTINATION ${RD_IMAGE_INSTALL_SERVERS})
