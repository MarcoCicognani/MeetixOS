add_executable(Read Read.cc)
target_link_libraries(Read LibApi LibC LibUtils LibIO LibTasking)
install(TARGETS Read DESTINATION Bins)
