add_subdirectory(circle)
add_subdirectory(grid)
add_subdirectory(spiralFill)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(markGraphLib ${DIR_SYSTEM_SRCS})

target_link_libraries(markGraphLib
    circleLib
    gridLib
    spiralFillLib)