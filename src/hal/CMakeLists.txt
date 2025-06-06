add_definitions( -D_ProjectName=\"${projectName}\")
add_subdirectory(plc)
add_subdirectory(megatron)
add_subdirectory(vm)
add_subdirectory(printDatas)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(halLib ${DIR_SYSTEM_SRCS})

target_link_libraries(halLib
    plcLib
    SLMVmLib
    megatronLib
    PrintDatasLib)