add_subdirectory(lineFill)
add_subdirectory(contourFill)
add_subdirectory(blockFill)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(scanPathLib ${DIR_SYSTEM_SRCS})

target_link_libraries(scanPathLib
    lineFillLib
    contourFillLib
    blockFillLib)