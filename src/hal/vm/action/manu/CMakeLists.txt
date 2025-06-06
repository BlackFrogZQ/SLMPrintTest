add_subdirectory(manuOnce)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(manuLib ${DIR_SYSTEM_SRCS})

target_link_libraries(manuLib
    manuOnceLib)