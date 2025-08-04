add_subdirectory(markGraph)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(scanSystemWindowLib ${DIR_SYSTEM_SRCS})

target_link_libraries(scanSystemWindowLib
    markGraphLib)