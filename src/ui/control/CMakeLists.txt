add_subdirectory(btnSmart)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(controlWindowLib ${DIR_SYSTEM_SRCS})
target_link_libraries(controlWindowLib
    btnSmartLib)