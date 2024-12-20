cmake_minimum_required(VERSION 3.27.0)
set(projectName ST_MarkDemo_CPP_Test CACHE STRING "项目名" FORCE)
set(projectVersion 0.1.0 CACHE STRING "版本号" FORCE)
project(${projectName} VERSION ${projectVersion})

# add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_STANDARD 17)
execute_process(
    COMMAND chcp 65001
)

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
SET(CMAKE_MODULE_PATH $ENV{QT_Dir})

include_directories($ENV{QT_Dir}\\include)
include_directories($ENV{QT_Dir}\\include\\QtGui)
include_directories($ENV{QT_Dir}\\include\\QtWidgets)
include_directories($ENV{QT_Dir}\\include\\QtCore)
include_directories($ENV{QT_Dir}\\include\\QtNetwork)
include_directories($ENV{QT_Dir}\\include\\QtSerialBus)
include_directories($ENV{QT_Dir}\\include\\QtSerialPort)
include_directories($ENV{QT_Dir}\\include\\QtConcurrent)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 5.15.2 REQUIRED COMPONENTS Core Gui Network SerialBus Widgets SerialPort Concurrent Qml )
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

add_definitions(-DNOMINMAX)
message("Looking for Qt...")
find_package(Qt5 COMPONENTS Core Gui Widgets Qml Network REQUIRED)
if(${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

set(Qt5model
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::SerialBus
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Concurrent
    Qt5::Qml)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/HMHaShu)
include_directories(${PROJECT_SOURCE_DIR}/include/hikvision)
file(GLOB libs "lib/HMHaShu/*.lib" "lib/hikvision/*.lib")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/ui)
include_directories(${PROJECT_SOURCE_DIR}/src/system)
include_directories(${PROJECT_SOURCE_DIR}/src/hal)
include_directories(${PROJECT_SOURCE_DIR}/src/para)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/ui)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/system)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/hal)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/para)

qt5_add_resources(sources qml.qrc)
set(versionRc "${PROJECT_SOURCE_DIR}/build/info.rc")
configure_file("${PROJECT_SOURCE_DIR}/info.rc.in" "${versionRc}")

if(isRelease)
    add_executable(${PROJECT_NAME}
        WIN32
        main.cpp
        ${sources}
        ${versionRc}
        )
else()
    add_executable(${PROJECT_NAME}
        main.cpp
        ${sources}
        ${versionRc}
        )
endif()

macro(Macro_LinkLib exeName)
    target_link_libraries(${exeName}
    ${Qt5model}
    ${libs}
    uiLib
    sysLib
    halLib
    paraLib
    )
endmacro()
Macro_LinkLib(${PROJECT_NAME})

# 打包
if(DEFINED CMAKE_BUILD_TYPE)
    set(exeDir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    # 所有非Qt dll库 不一定在统一目录 可直接追加目录
    file(GLOB libDlls
        "${PROJECT_SOURCE_DIR}/lib/HMHaShu/*.dll"
        "${PROJECT_SOURCE_DIR}/lib/hikvision/*.dll")
    message("${PROJECT_NAME}.exe 所在目录:" ${exeDir})
    message("拷贝 libDlls 库到程序运行目录...")
    string(REPLACE ";" "\n\t" outLibDllNames "${libDlls}")
    message("libDlls:\n\t${outLibDllNames}")
    file(COPY ${libDlls} DESTINATION ${exeDir})
    message("拷贝结束...")
    message("运行qt打包程序...")
    execute_process(
        COMMAND windeployqt ${exeDir}\\${PROJECT_NAME}.exe
        WORKING_DIRECTORY $ENV{QT5.14.0}\\bin
    )
    message("qt打包结束")
else()
    message("未在用户设置中设置 'cmake.setBuildTypeOnMultiConfig'为 true,无法打包")
endif()