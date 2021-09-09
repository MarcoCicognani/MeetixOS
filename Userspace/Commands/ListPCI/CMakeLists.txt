add_executable(ListPCI ListPCI.c list.c)
target_include_directories(ListPCI PRIVATE .)
install(TARGETS ListPCI DESTINATION ${RD_IMAGE_INSTALL_BINS})
