cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(ClassSystem LANGUAGES CXX VERSION 0.1.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "c:/Qt/5.15.2/mingw81_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets Quick Core REQUIRED) # Qt COMPONENTS

set(PROJECT_VERSION_STR "\"${PROJECT_VERSION}\"")
string(TIMESTAMP COMPILE_TIME "\"%Y-%m-%d %H:%M:%S\"")
configure_file(src/config.h.in config.h)

aux_source_directory(./src/ClassSystem ClassSystemSrc)
aux_source_directory(./src/ClassData ClasDataSrc)
set(APP_CMD_TYPE WIN32)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(APP_CMD_TYPE)
endif()

include_directories(src/ClassData)

# target ClassSystem BEGIN
add_executable(ClassSystem
    ${APP_CMD_TYPE}
    ${ClassSystemSrc}
    ${ClasDataSrc}
)
target_include_directories(ClassSystem PUBLIC src/ClassSystem)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Quick) # Qt5 Shared Libraries
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

# target ClassSystem END
