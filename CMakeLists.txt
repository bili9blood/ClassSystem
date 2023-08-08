cmake_minimum_required(VERSION 3.5)
project(ClassSystem LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets Core Network WinExtras REQUIRED)

string(TIMESTAMP COMPILE_TIME "\"%Y-%m-%d %H:%M:%S\"")
configure_file(src/config.h.in config.h)

set(ClassSystemDir ${CMAKE_SOURCE_DIR}/src/ClassSystem)
set(APP_CMD_TYPE WIN32)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(APP_CMD_TYPE)
endif()

include_directories(src ${CMAKE_BINARY_DIR} thirdparty)

# target ClassSystem BEGIN
add_executable(ClassSystem

    ${APP_CMD_TYPE} # `WIN32` or None

    assets/ClassSystem.qrc
    src/ClassSystem/main.cpp

    src/ClassSystem/Widgets/PopupMenu.cpp src/ClassSystem/Widgets/PopupMenu.h
    src/ClassSystem/Widgets/MenuButton.cpp src/ClassSystem/Widgets/MenuButton.h

    src/ClassSystem/Widgets/ScreenCapturer.cpp src/ClassSystem/Widgets/ScreenCapturer.h
    src/ClassSystem/Widgets/ImgPaster.cpp src/ClassSystem/Widgets/ImgPaster.h

    src/ClassSystem/Widgets/MainPanel.cpp src/ClassSystem/Widgets/MainPanel.h

    thirdparty/QAnimationStackedWidget/QAnimationStackedWidget.cpp thirdparty/QAnimationStackedWidget/QAnimationStackedWidget.h

    src/ClassData.h
)
target_include_directories(ClassSystem PUBLIC src/ClassSystem)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

# target ClassSystem END
