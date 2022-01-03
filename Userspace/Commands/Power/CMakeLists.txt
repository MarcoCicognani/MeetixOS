add_executable(Power Power.cc)
target_link_libraries(Power LibApi LibC LibUtils)
install(TARGETS Power DESTINATION Bins)
