add_subdirectory(graphicsWindow)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(imageListPanelLib ${DIR_SYSTEM_SRCS})
target_link_libraries(imageListPanelLib
    graphicsLib)