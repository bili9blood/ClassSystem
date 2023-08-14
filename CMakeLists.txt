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

include_directories(src ${CMAKE_BINARY_DIR} thirdparty thirdparty/QXlsx/header)

set(UTILS_SOURCES src/ClassData.h)

# target ClassSystem BEGIN
add_executable(ClassSystem

    assets/ICON.RC

    ${APP_CMD_TYPE} # `WIN32` or None

    assets/ClassSystem.qrc

    src/ClassSystem/main.cpp

    src/ClassSystem/PopupMenu.cpp src/ClassSystem/PopupMenu.h
    src/ClassSystem/MenuButton.cpp src/ClassSystem/MenuButton.h

    src/ClassSystem/ScreenCapturer.cpp src/ClassSystem/ScreenCapturer.h
    src/ClassSystem/ImgPaster.cpp src/ClassSystem/ImgPaster.h

    src/ClassSystem/MainPanel.cpp src/ClassSystem/MainPanel.h

    src/ClassSystem/TitleWidget.cpp src/ClassSystem/TitleWidget.h

    src/ClassSystem/TableWindow.cpp src/ClassSystem/TableWindow.h

    thirdparty/QAnimationStackedWidget/QAnimationStackedWidget.cpp thirdparty/QAnimationStackedWidget/QAnimationStackedWidget.h

    ${UTILS_SOURCES}
)
target_include_directories(ClassSystem PUBLIC src/ClassSystem)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

# target ClassSystem END

# target ClassAdmin BEGIN
qt_wrap_ui(UI_SOURCES src/ClassAdmin/MainWindow.ui)
add_executable(ClassAdmin

    assets/ICON.RC

    ${APP_CMD_TYPE} # `WIN32` or None

    assets/ClassAdmin.qrc

    src/ClassAdmin/main.cpp

    src/ClassAdmin/MainWindow.cpp src/ClassAdmin/MainWindow.h

    src/ClassAdmin/ImportDialog.cpp src/ClassAdmin/ImportDialog.h

    src/ClassAdmin/ResetPwdDialog.cpp src/ClassAdmin/ResetPwdDialog.h

    ${UI_SOURCES}

    ${UTILS_SOURCES}
)
target_include_directories(ClassAdmin PUBLIC src/ClassAdmin)
target_link_libraries(ClassAdmin PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassAdmin PUBLIC src/pch.h)

# target ClassAdmin END