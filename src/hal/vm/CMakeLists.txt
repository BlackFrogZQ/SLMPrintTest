set(CMAKE_AUTOMOC ON)
add_subdirectory(action)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(SLMVmLib ${DIR_SYSTEM_SRCS})

target_link_libraries(SLMVmLib
        actionLib)