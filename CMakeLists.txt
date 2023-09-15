cmake_minimum_required(VERSION 3.5)

cmake_policy(SET CMP0091 NEW)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_Debug)
else()
    set(__DECL_RELEASE_WIN32 WIN32)
endif()

include_directories(src ${CMAKE_BINARY_DIR} include)

set(UTILS_SOURCES src/ClassData.h)

# 从 version.txt 中读取版本
file(STRINGS version.txt VER LIMIT_COUNT 1)
string(LENGTH ${VER} VER_LENGTH)
string(SUBSTRING ${VER} 1 ${VER_LENGTH}-1 VER)

project(ClassSystem LANGUAGES CXX VERSION ${VER})

configure_file(src/config.h.in config.h)

# 配置 Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets Core Network REQUIRED)

# ----------------------------------------------------------------
# ClassSystem 前台展示端
# ----------------------------------------------------------------
qt_wrap_ui(
    CLASSSYSTEM_UI_SOURCES

    src/ClassSystem/RollCallWindow.ui
)
add_executable(ClassSystem

    ${__DECL_RELEASE_WIN32}

    # 资源
    assets/ICON.RC # Windows Only
    assets/ClassSystem.qrc

    src/ClassSystem/main.cpp

    # 主面板
    src/ClassSystem/MainPanel.cpp src/ClassSystem/MainPanel.h

    # 悬停菜单
    src/ClassSystem/PopupMenu.cpp src/ClassSystem/PopupMenu.h
    src/ClassSystem/MenuButton.cpp src/ClassSystem/MenuButton.h

    # 截图&贴图
    src/ClassSystem/ScreenCapturer.cpp src/ClassSystem/ScreenCapturer.h
    src/ClassSystem/ImgPaster.cpp src/ClassSystem/ImgPaster.h

    # 标题
    src/ClassSystem/TitleWidget.cpp src/ClassSystem/TitleWidget.h

    # 表格
    src/ClassSystem/TableWindow.cpp src/ClassSystem/TableWindow.h

    # 随机点名
    src/ClassSystem/RollCallWindow.cpp src/ClassSystem/RollCallWindow.h

    # 滑动 Stacked Widget
    include/QAnimationStackedWidget/QAnimationStackedWidget.cpp
    include/QAnimationStackedWidget/QAnimationStackedWidget.h

    ${CLASSSYSTEM_UI_SOURCES}
    ${UTILS_SOURCES}
)
target_include_directories(ClassSystem PUBLIC src/ClassSystem)
target_link_libraries(ClassSystem PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassSystem PUBLIC src/pch.h)

# ----------------------------------------------------------------
# ClassAdmin 后台管理端
# ----------------------------------------------------------------
qt_wrap_ui(
    CLASSADMIN_UI_SOURCES

    src/ClassAdmin/MainWindow.ui # 主窗口
    src/ClassAdmin/EditMealStuDialog.ui # 编辑抬饭生
    src/ClassAdmin/StuOnDutyCellWdiget.ui # 值日生单元格
    src/ClassAdmin/NoticesCellWidget.ui # 公告单元格
)
add_executable(ClassAdmin

    ${__DECL_RELEASE_WIN32}

    # 资源
    assets/ICON.RC # Windows Only
    assets/ClassAdmin.qrc

    src/ClassAdmin/main.cpp

    # 主窗口
    src/ClassAdmin/MainWindow.cpp src/ClassAdmin/MainWindow.h

    # 学生
    src/ClassAdmin/ImportDialog.cpp src/ClassAdmin/ImportDialog.h
    src/ClassAdmin/EditMealStuDialog.cpp src/ClassAdmin/EditMealStuDialog.h
    src/ClassAdmin/StuOnDutyCellWidget.cpp src/ClassAdmin/StuOnDutyCellWidget.h

    # 公告单元格
    src/ClassAdmin/NoticesCellWidget.cpp src/ClassAdmin/NoticesCellWidget.h

    # 工具栏
    src/ClassAdmin/ResetPwdDialog.cpp src/ClassAdmin/ResetPwdDialog.h
    src/ClassAdmin/Commands.h

    # 自定义代理
    src/ClassAdmin/ItemDelegates.h

    ${CLASSADMIN_UI_SOURCES}
    ${UTILS_SOURCES}
)
target_link_directories(ClassAdmin PUBLIC lib)
target_link_libraries(ClassAdmin PUBLIC "quazip1-qt5$<$<CONFIG:Debug>:d>.lib")

target_include_directories(ClassAdmin PUBLIC src/ClassAdmin)
target_link_libraries(ClassAdmin PUBLIC Qt5::Widgets Qt5::Core Qt5::Network)
target_precompile_headers(ClassAdmin PUBLIC src/pch.h)
