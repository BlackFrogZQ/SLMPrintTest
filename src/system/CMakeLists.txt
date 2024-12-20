add_definitions( -D_ProjectName=\"${projectName}\")
add_subdirectory(logSystem)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(sysLib ${DIR_SYSTEM_SRCS})

target_link_libraries(sysLib
    systemLogLib)