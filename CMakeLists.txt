﻿cmake_minimum_required(VERSION 3.27.0)
set(projectName SLMPrintTest CACHE STRING "项目名" FORCE)
set(projectVersion 0.1.0 CACHE STRING "版本号" FORCE)
project(${projectName} VERSION ${projectVersion} LANGUAGES CXX)

# add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# set(CMAKE_CXX_STANDARD 17)
# execute_process(
#     COMMAND chcp 65001
# )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(DEFINED CMAKE_BUILD_TYPE)
    if(${_Debug})
        set(isRelease OFF CACHE BOOL "relase版本标志" FORCE)
        message(STATUS "Debug版")
        add_definitions(-DTrace)
    elseif(${_Release})
        set(isRelease ON CACHE BOOL "relase版本标志" FORCE)
        message(STATUS "relase版")
    endif()
else()
    message(FATAL_ERROR "未在setting.json中设置 'cmake.setBuildTypeOnMultiConfig'为 true,无法生成")
endif()

# 打开我的电脑>属性>高级系统设置>环境变量
# 新建用户变量 变量名"QT_Dir"，变量值"qt5.12.12下编译器的目录，比如我的是'C:\Qt\Qt5.12.12\5.12.12\msvc2017_64'"
# 同理 新建用户变量 变量名"OrionLib"，变量值"OrionLib所在目录，比如我的是在'C:\OrionLib'"
# 确定保存即可，cmake访问系统变量 $ENV{变量名},如:访问上述变量'QT_Dir变量' $ENV{QT_Dir}
# 添加用户变量后需重启
SET(CMAKE_PREFIX_PATH $ENV{QT_Dir})
include_directories(${PROJECT_SOURCE_DIR})
include_directories($ENV{QT_Dir}\\include)
include_directories($ENV{QT_Dir}\\include\\QtGui)
include_directories($ENV{QT_Dir}\\include\\QtWidgets)
include_directories($ENV{QT_Dir}\\include\\QtCore)
include_directories($ENV{QT_Dir}\\include\\QtNetwork)
include_directories($ENV{QT_Dir}\\include\\QtSerialBus)
include_directories($ENV{QT_Dir}\\include\\QtSerialPort)
include_directories($ENV{QT_Dir}\\include\\QtConcurrent)
include_directories($ENV{QT_Dir}\\include\\QtMultimedia)
include_directories($ENV{QT_Dir}\\include\\QtMultimediaWidgets)
include_directories($ENV{QT_Dir}\\include\\QtOpenGL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_definitions(-DNOMINMAX)
message("Looking for Qt...")
find_package(Qt5 COMPONENTS Core Gui Network SerialBus Widgets SerialPort Concurrent Multimedia MultimediaWidgets  OpenGL Qml REQUIRED)

if(${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

execute_process(
    COMMAND chcp 65001
)

set(Qt5model
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialBus
    Qt5::Gui
    Qt5::Core
    Qt5::SerialPort
    Qt5::Concurrent
    Qt5::OpenGL
    opengl32)

set(isRelease TRUE)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(isRelease FALSE)
    add_definitions(-DTrace)
endif()

file(GLOB CLIPPER2_SRC "${PROJECT_SOURCE_DIR}/include/clipper2/*.cpp" "${PROJECT_SOURCE_DIR}/include/clipper2/*.h")
file(GLOB HMHaShuLIBS "${PROJECT_SOURCE_DIR}/lib/HMHaShu/*.lib")
file(GLOB HMHaShuDlls "${PROJECT_SOURCE_DIR}/lib/HMHaShu/*.dll")
file(GLOB clipper2LIBS "${PROJECT_SOURCE_DIR}/lib/clipper2/*.lib")
file(GLOB clipper2Dlls "${PROJECT_SOURCE_DIR}/lib/clipper2/*.dll")
# 所有非Qt通用dll库 不一定在统一目录 可直接追加目录
# set_property(GLOBAL APPEND PROPERTY LIBS ${HMHaShuLIBS})
set_property(GLOBAL APPEND PROPERTY LIBS ${HMHaShuLIBS} ${clipper2LIBS})
# 所有非Qt通用dll库 不一定在统一目录 可直接追加目录
# set_property(GLOBAL APPEND PROPERTY libDlls ${HMHaShuDlls})
set_property(GLOBAL APPEND PROPERTY libDlls ${HMHaShuDlls} ${clipper2Dlls})
get_property(currentLibs GLOBAL PROPERTY LIBS)

macro(Macro_SetOutPath outPath)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${outPath})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${outPath}/libs)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${outPath}/libs)
    set(LIBRARY_OUTPUT_PATH ${outPath}/libs)
endmacro()
Macro_SetOutPath(${CMAKE_CURRENT_BINARY_DIR})

include_directories(./)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/HMHaShu)
include_directories(${PROJECT_SOURCE_DIR}/include/clipper2)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/ui)
include_directories(${PROJECT_SOURCE_DIR}/src/system)
include_directories(${PROJECT_SOURCE_DIR}/src/hal)
include_directories(${PROJECT_SOURCE_DIR}/src/para)
include_directories(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/ui)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/system)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/hal)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/para)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)

qt5_add_resources(sources qml.qrc)
set(versionRc "${PROJECT_SOURCE_DIR}/build/info.rc")
configure_file("${PROJECT_SOURCE_DIR}/info.rc.in" "${versionRc}")

if(isRelease)
    add_executable(${PROJECT_NAME}
        WIN32
        main.cpp
        ${sources}
        ${versionRc}
        ${CLIPPER2_SRC}
        )
else()
    add_executable(${PROJECT_NAME}
        WIN32
        main.cpp
        ${sources}
        ${versionRc}
        ${CLIPPER2_SRC}
        )
endif()

macro(Macro_LinkLib exeName)
    target_link_libraries(${exeName}
    ${Qt5model}
    ${currentLibs}
    qtSingleApplicationLib
    uiLib
    sysLib
    halLib
    paraLib
    )
endmacro()
Macro_LinkLib(${PROJECT_NAME})

# 打包
function(Fun_CopyDll exeBasicName)
    set(exeDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
    message(STATUS "${exeBasicName}.exe 所在目录:" ${exeDir})
    file(GLOB currentDlls "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/*.dll")

    foreach(currentDll ${currentDlls})
        file(REMOVE ${currentDll})
    endforeach()

    get_property(currentDlls GLOBAL PROPERTY libDlls)
    string(REPLACE ";" "\n\t" outLibDllNames "${currentDlls}")
    message(STATUS "拷贝 libDlls 库...\n\t${outLibDllNames}")
    file(COPY ${currentDlls} DESTINATION ${exeDir})
    message(STATUS "拷贝结束...")
    message(STATUS "运行qt打包程序...")
    execute_process(
        COMMAND windeployqt ${exeDir}\\${exeBasicName}.exe
        WORKING_DIRECTORY $ENV{QT_Dir}\\bin
        OUTPUT_QUIET

        # ERROR_QUIET
        ERROR_STRIP_TRAILING_WHITESPACE
        ERROR_VARIABLE errorMsg
    )

    if("${errorMsg}" STREQUAL "")
        message(STATUS "qt打包成功...")
    else()
        message(STATUS "copy error:${errorMsg}")
        message(WARNING "qt打包失败...请按F7生成exe后,重新运行cmake(在此文件中ctrl+s)")
    endif()
endfunction()

Fun_CopyDll(${PROJECT_NAME})


# # 打包
# if(DEFINED CMAKE_BUILD_TYPE)
#     set(exeDir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
#     # 所有非Qt dll库 不一定在统一目录 可直接追加目录
#     file(GLOB libDlls
#         "${PROJECT_SOURCE_DIR}/lib/HMHaShu/*.dll"
#         "${PROJECT_SOURCE_DIR}/lib/hikvision/*.dll")
#     message("${PROJECT_NAME}.exe 所在目录:" ${exeDir})
#     message("拷贝 libDlls 库到程序运行目录...")
#     string(REPLACE ";" "\n\t" outLibDllNames "${libDlls}")
#     message("libDlls:\n\t${outLibDllNames}")
#     file(COPY ${libDlls} DESTINATION ${exeDir})
#     message("拷贝结束...")
#     message("运行qt打包程序...")
#     execute_process(
#         COMMAND windeployqt ${exeDir}\\${PROJECT_NAME}.exe
#         WORKING_DIRECTORY $ENV{QT5.14.0}\\bin
#     )
#     message("qt打包结束")
# else()
#     message("未在用户设置中设置 'cmake.setBuildTypeOnMultiConfig'为 true,无法打包")
# endif()