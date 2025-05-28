aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(SLMVmLib ${DIR_SYSTEM_SRCS})

add_subdirectory(action)

target_link_libraries(SLMVmLib
    actionLib)