add_subdirectory(printMegatron)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(megatronLib ${DIR_SYSTEM_SRCS})

target_link_libraries(megatronLib
    printMegatronLib)
