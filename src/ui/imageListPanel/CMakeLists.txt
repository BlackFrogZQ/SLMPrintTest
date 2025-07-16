add_subdirectory(graphicsWindow)
add_subdirectory(openGLWindow)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(imageListPanelLib ${DIR_SYSTEM_SRCS})
target_link_libraries(imageListPanelLib
    graphicsLib
    openGLWindowLib)