cmake_minimum_required(VERSION 4.0)
project(PLauncher)
# 启用 Unicode 模式（Windows 平台）
if (WIN32)
    add_compile_definitions(UNICODE _UNICODE)
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        OpenGL
        SerialPort
        Sql
        Svg
        WebChannel
        WebSockets
        Multimedia
        Concurrent
        MultimediaWidgets
        REQUIRED)

# 设置是否为debug模式
set(D_OR_R OFF)#ON for Debug, OFF for Release

# 是否开启控制台调试输出
set(CONSOLE OFF)
#设置渲染方式，选择为0penGL，不然默认是Cocos2d，编译包报错的，CMake也会给你警告的
set(FRAMEWORK_SOURCE OpenGL)
qt_add_resources(QT_RESOURCES
        Resource.qrc
)
FILE(GLOB SOURCE_FILES "./src/sources/*.cpp")
FILE(GLOB UI_FILES "./src/sources/*.ui")
FILE(GLOB HEADER_FILES "./src/headers/*.h")
FILE(GLOB HPP_FILES "./src/headers/*.hpp")
file(GLOB LApp2D "LAppLive2D/src/*.cpp")
message(STATUS ${LApp2D})

# 头文件搜索路径
include_directories(
        ./
        src/headers
        LAppLive2D/inc
        thirdParty/Core/include
        thirdParty/Framework/src
        thirdParty/glew/include
        thirdParty/glew/include/GL
        thirdParty/glfw/include
        thirdParty/glfw/include/GLFW
        thirdParty/stb
)
link_directories(
        "${CMAKE_SOURCE_DIR}/lib/Debug"
)


if (NOT CONSOLE)
    set(CON WIN32)
endif ()
# 包含目录
include_directories(PLauncher PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}  # 重要：包含构建目录，ui_*.h文件在这里生成
)

add_executable(PLauncher
        ${CON}
        ${UI_FILES}
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${HPP_FILES}
        ${LApp2D}
        ${QT_RESOURCES}
)
target_link_libraries(PLauncher
        Framework
        libglew32d.dll
        glfw3
        Live2DCubismCore_d
        opengl32
        glu32
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::OpenGL
        Qt5::SerialPort
        Qt5::Sql
        Qt5::Svg
        Qt5::WebChannel
        Qt5::WebSockets
        Qt5::Multimedia
        Qt5::Concurrent
        Qt5::MultimediaWidgets
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
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
