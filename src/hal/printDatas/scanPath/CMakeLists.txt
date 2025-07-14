add_subdirectory(fillType)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(scanPathLib ${DIR_SYSTEM_SRCS})

target_link_libraries(scanPathLib
    fillTypeLib)