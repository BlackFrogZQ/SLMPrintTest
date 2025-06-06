add_subdirectory(autoWorkOnce)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(autoWorkActionLib ${DIR_SYSTEM_SRCS})

target_link_libraries(autoWorkActionLib
    autoWorkOnceLib)