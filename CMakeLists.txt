cmake_minimum_required(VERSION 3.5)
project(ClassSystem LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "c:/Qt/5.15.2/mingw81_64")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets Core Network WinExtras REQUIRED)

string(TIMESTAMP COMPILE_TIME "\"%Y-%m-%d %H:%M:%S\"")
configure_file(src/config.h.in config.h)

set(ClassSystemDir ${CMAKE_SOURCE_DIR}/src/ClassSystem)
set(APP_CMD_TYPE WIN32)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(APP_CMD_TYPE)
endif ()

include_directories(src/ClassData ${CMAKE_BINARY_DIR})

# target ClassSystem BEGIN
add_executable(ClassSystem
        ${APP_CMD_TYPE}
        assets/ClassSystem.qrc
        src/ClassSystem/main.cpp
        src/ClassSystem/Widgets/SuspendedWidget.cpp
        src/ClassSystem/Widgets/SuspendedWidget.h
        src/ClassSystem/Widgets/MenuWidget.cpp
        src/ClassSystem/Widgets/MenuWidget.h
        src/ClassSystem/Widgets/MainPanel.cpp src/ClassSystem/Widgets/MainPanel.h
        src/ClassData.h src/ClassSystem/Core/Updater.cpp src/ClassSystem/Core/Updater.h src/token.h src/token.h src/ClassSystem/Widgets/UpdateDialog.cpp src/ClassSystem/Widgets/UpdateDialog.h src/ClassSystem/Core/DownloadWorker.cpp src/ClassSystem/Core/DownloadWorker.h)
target_include_directories(ClassSystem PUBLIC src/ClassSystem)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Network Qt5::WinExtras)
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

# target ClassSystem END
