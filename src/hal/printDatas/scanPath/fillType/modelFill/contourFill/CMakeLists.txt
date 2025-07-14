aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(contourFillLib ${DIR_SYSTEM_SRCS})

target_link_libraries(contourFillLib)