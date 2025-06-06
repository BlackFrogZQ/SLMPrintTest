aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(PrintDatasLib ${DIR_SYSTEM_SRCS})

target_link_libraries(PrintDatasLib)