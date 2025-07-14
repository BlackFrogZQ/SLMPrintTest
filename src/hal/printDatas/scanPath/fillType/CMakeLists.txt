add_subdirectory(modelFill)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(fillTypeLib ${DIR_SYSTEM_SRCS})

target_link_libraries(fillTypeLib
    modelLib)