add_subdirectory(autoWork)
add_subdirectory(markOnce)
add_subdirectory(spread)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(actionLib ${DIR_SYSTEM_SRCS})

target_link_libraries(actionLib
    autoWorkActionLib
    markOnceLib
    spreadLib)