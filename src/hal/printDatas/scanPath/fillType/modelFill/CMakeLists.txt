add_subdirectory(lineFill)
add_subdirectory(contourFill)
add_subdirectory(blockFill)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(modelLib ${DIR_SYSTEM_SRCS})

target_link_libraries(modelLib
    lineFillLib
    contourFillLib
    blockFillLib)