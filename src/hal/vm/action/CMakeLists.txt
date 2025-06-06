add_subdirectory(manu)
add_subdirectory(mark)
add_subdirectory(spread)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(actionLib ${DIR_SYSTEM_SRCS})

target_link_libraries(actionLib
    manuLib
    markLib
    spreadLib)