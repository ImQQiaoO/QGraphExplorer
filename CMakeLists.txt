cmake_minimum_required(VERSION 3.20)
project(QGraphExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set(CMAKE_PREFIX_PATH "D:/Qt/6.7.2/mingw_64")
# 避免hard code，请在CLion设置中CMake options中写入-DCMAKE_PREFIX_PATH="D:/Qt/6.7.2/mingw_64"

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(FILE_ROOT_DIR "${CMAKE_SOURCE_DIR}")
add_definitions(-DROOT_DIR="${FILE_ROOT_DIR}")

# 引入 FetchContent 模块
include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(spdlog nlohmann_json)

add_executable(QGraphExplorer
        src/main.cpp
        src/MainWindow.cpp
        src/GraphWidget.cpp
        src/EdgeItem.cpp
        src/VertexItem.cpp
        src/AddItems.cpp
        src/VertexWithInfo.cpp
        src/VertexInfoPopup.cpp
        src/QuadTree.cpp
        src/utils/Locale.hpp
        src/Movie.cpp
        src/JSONProcessor.cpp
)

target_compile_definitions(QGraphExplorer PRIVATE BUILD_WITH_CMAKE)

target_link_libraries(QGraphExplorer
        Qt::Core
        Qt::Gui
        Qt::Widgets
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
