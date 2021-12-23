add_executable(Stress
        Stress.cc
        Tests/Test.cc
        Tests/FileSystem/Create.cc
        Tests/FileSystem/OpenClose.cc
        Tests/FileSystem/ReadWrite.cc
        Tests/Memory/Heap.cc
        Tests/Memory/Stack.cc
        Tests/Threads/Native.cc
        Tests/Threads/POSIX.cc Tests/Manager.cc)
target_include_directories(Stress PRIVATE .)
target_link_libraries(Stress LibApi LibC LibUtils)
install(TARGETS Stress DESTINATION ${RD_IMAGE_INSTALL_BINS})
