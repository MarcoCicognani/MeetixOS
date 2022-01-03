add_executable(FileId FileId.cc Inspector.cc ELFInspector.cc ShellInspector.cc TextInspector.cc)
target_link_libraries(FileId LibApi LibC LibUtils)
install(TARGETS FileId DESTINATION Bins)
