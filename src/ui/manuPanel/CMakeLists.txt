aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(manuPanelLib ${DIR_SYSTEM_SRCS})

target_link_libraries(manuPanelLib)