cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0091 NEW)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_Debug)
else()
    set(__DECL_RELEASE_WIN32 WIN32)
endif()

include_directories(src ${CMAKE_BINARY_DIR} include)

set(UTILS_SOURCES src/ClassData.h)

project(ClassSystem LANGUAGES CXX VERSION 1.9.1)

configure_file(src/config.h.in config.h)

# 配置 Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets Core Network REQUIRED)

qt_wrap_ui(
    CLASSSYSTEM_UI_SOURCES

    src/RollCallWindow.ui
)
add_executable(ClassSystem

    ${__DECL_RELEASE_WIN32}

    # 资源
    assets/ICON.RC # Windows Only
    assets/ClassSystem.qrc

    src/main.cpp

    # 主面板
    src/MainPanel.cpp src/MainPanel.h

    # 显示版本，有更新时显示下载进度
    src/VersionDisplay.cpp src/VersionDisplay.h

    # 悬停菜单
    src/PopupMenu.cpp src/PopupMenu.h
    src/MenuButton.cpp src/MenuButton.h

    # 截图&贴图
    src/ScreenCapturer.cpp src/ScreenCapturer.h
    src/ImgPaster.cpp src/ImgPaster.h

    # 标题
    src/TitleWidget.cpp src/TitleWidget.h

    # 表格
    src/TableWindow.cpp src/TableWindow.h

    # 随机点名
    src/RollCallWindow.cpp src/RollCallWindow.h

    # 滑动 Stacked Widget
    include/QAnimationStackedWidget/QAnimationStackedWidget.cpp
    include/QAnimationStackedWidget/QAnimationStackedWidget.h

    ${CLASSSYSTEM_UI_SOURCES}
    ${UTILS_SOURCES}
)
target_include_directories(ClassSystem PUBLIC src)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

add_executable(Updater ${__DECL_RELEASE_WIN32} src/UpdaterMain.cpp)

if(${MSVC})
    set_target_properties(ClassSystem Updater PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()