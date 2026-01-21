# PLauncher - Live2D Virtual Desktop Partner
# https://gitee.com/Pfolg/plauncher
# https://sourceforge.net/projects/pfolg-plauncher/
# Copyright (c) 2025 SY Cheng
#
# GPL v3 License
# https://gnu.ac.cn/licenses/gpl-3.0.html

cmake_minimum_required(VERSION 4.0)
project(PLauncher)
# ------------------
# 设置是否为debug模式
set(D_OR_R OFF)#ON for Debug, OFF for Release

# 是否开启控制台调试输出
set(CONSOLE OFF)
# ------------------

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
        LinguistTools
        REQUIRED)

# @Translations
#FILE(GLOB TRANSLATION_FILES "./translations/*.ts")
# 定义翻译文件（.ts）列表，自定义语言（如zh_CN=中文，en_US=英文）
# 注意：文件路径可以自定义，这里放在项目根目录的translations文件夹下
set(TRANSLATION_FILES
        translations/language_zh_CN.ts
        translations/language_en_US.ts
)


#设置渲染方式，选择为0penGL，不然默认是Cocos2d，编译包报错的，CMake也会给你警告的
set(FRAMEWORK_SOURCE OpenGL)
qt_add_resources(QT_RESOURCES
        Resource.qrc
)
FILE(GLOB SOURCE_FILES "./src/sources/*.cpp")
FILE(GLOB UI_FILES "./src/ui/*.ui")
FILE(GLOB HEADER_FILES "./src/headers/*.h")
FILE(GLOB HPP_FILES "./src/headers/*.hpp")
file(GLOB LApp2D "LAppLive2D/src/*.cpp")
qt5_wrap_ui(UI_HEADERS ${UI_FILES})
message(STATUS ${LApp2D})

# @Translations
# Qt5的MOC、UIC、RCC工具处理（必须，Qt5项目标配）
qt5_wrap_cpp(MOC_SOURCES ${HEADER_FILES} ${HPP_FILES})
qt5_wrap_ui(UI_SOURCES ${UI_FILES})

# @Translations
# 使用lupdate生成/更新.ts文件：扫描源文件、头文件、UI文件中的tr()字符串
# 第一个参数：生成的QM文件变量（后续用），第二个参数：TS文件列表，第三个参数：需要扫描的文件
qt5_create_translation(QM_FILES ${SOURCE_FILES} ${UI_FILES} ${HEADER_FILES} ${HPP_FILES} ${TRANSLATION_FILES})

# 头文件搜索路径
include_directories(
        ./
        src/headers
        src/ui
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
# 明确设置 UI 文件目录
set(UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ui")

include_directories(PLauncher PRIVATE
        ${UI_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}  # 重要：包含构建目录，ui_*.h文件在这里生成
)
# 确保 AUTOUIC 能找到 .ui 文件
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR}) # 没有这句就GG

add_executable(PLauncher
        ${CON}
        #        ${UI_FILES}
        ${SOURCE_FILES}
        #        ${HEADER_FILES}
        #        ${HPP_FILES}
        ${MOC_SOURCES}
        ${UI_SOURCES}
        ${LApp2D}
        ${QT_RESOURCES}
        ${QM_FILES}  # 将QM文件加入项目，确保编译时生成
)
target_link_libraries(PLauncher
        Framework
        libglew32d
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
# ===================== 正确的Windows程序图标设置 =====================
if (WIN32)
    # 1. 定义图标文件路径和rc脚本路径
    set(APP_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/image/PLauncher.ico")
    set(APP_RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/icon.rc")

    # 2. 检查图标和rc脚本是否存在
    if (EXISTS ${APP_ICON_FILE} AND EXISTS ${APP_RC_FILE})
        # 3. 将rc脚本文件加入项目编译（核心：通过rc脚本绑定图标）
        target_sources(${PROJECT_NAME} PRIVATE ${APP_RC_FILE})

        # 4. 配合你的CONSOLE开关，设置是否为无控制台的GUI程序（完美联动你的原有逻辑）
        if (NOT CONSOLE)
            set_target_properties(${PROJECT_NAME} PROPERTIES
                    WIN32_EXECUTABLE TRUE  # 无控制台窗口
            )
        else ()
            set_target_properties(${PROJECT_NAME} PROPERTIES
                    WIN32_EXECUTABLE FALSE # 带控制台窗口
            )
        endif ()
    else ()
        message(WARNING "Program icon file or RC script is missing, the default icon will be used!")
    endif ()
endif ()

# @Translations
# 问题：$<TARGET_FILE_DIR:${PROJECT_NAME}> 是生成器表达式，只能在构建阶段解析
# 解决方案：将「创建目录+复制文件」合并到 POST_BUILD 命令中，构建阶段执行
set(QM_OUTPUT_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations")
foreach (qm_file ${QM_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # 第一步：创建目录（构建阶段解析生成器表达式，路径有效）
            COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
            # 第二步：复制 QM 文件到目录
            COMMAND ${CMAKE_COMMAND} -E copy ${qm_file} ${QM_OUTPUT_DIR}/
            COMMENT "Copying ${qm_file} to ${QM_OUTPUT_DIR}"
    )
endforeach ()
# ==============================================
# Windows 平台专属：复制 Qt 运行时依赖（DLL + 平台插件）
# 核心修正：Qt 路径拼接逻辑，确保指向正确的 bin/plugins 目录
# ==============================================
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # 1. 配置 Debug 后缀（适配 MSVC/MinGW 双编译器）
    set(DEBUG_SUFFIX "")
    #    if (CMAKE_BUILD_TYPE MATCHES "Debug")
    #        set(DEBUG_SUFFIX "d")
    #    endif () # 由于我没有debug的动态库，所以注释掉了

    # 2. 正确设置 Qt 路径（关键修正！）
    # CMAKE_PREFIX_PATH = D:/Qt/5.15.2/mingw81_64（Qt 安装根目录）
    # Qt 的 DLL 在 bin 目录下，插件在 plugins 目录下（与 bin 同级）
    set(QT_BIN_PATH "${CMAKE_PREFIX_PATH}/bin")  # 直接指向 Qt 的 bin 目录
    set(QT_PLUGIN_PATH "${CMAKE_PREFIX_PATH}/plugins")  # 直接指向 Qt 的 plugins 目录

    # 路径有效性检查
    if (NOT EXISTS "${QT_BIN_PATH}")
        message(FATAL_ERROR "Qt bin directory does not exist! Path: ${QT_BIN_PATH}, please check if CMAKE_PREFIX_PATH is correct")
    endif ()
    if (NOT EXISTS "${QT_PLUGIN_PATH}")
        message(FATAL_ERROR "Qt plugins directory does not exist! Path: ${QT_PLUGIN_PATH}")
    endif ()

    # 3. 复制 Qt 平台插件（修正路径：直接用 QT_PLUGIN_PATH）
    set(QT_WINDOWS_PLUGIN "${QT_PLUGIN_PATH}/platforms/qwindows${DEBUG_SUFFIX}.dll")
    if (EXISTS "${QT_WINDOWS_PLUGIN}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                COMMENT "Create Qt platform plugin directory: plugins/platforms/"
        )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_WINDOWS_PLUGIN}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                COMMENT "Copy Qt platform plugin: qwindows${DEBUG_SUFFIX}.dll"
        )
    else ()
        message(FATAL_ERROR "Qt platform plugin missing! Path: ${QT_WINDOWS_PLUGIN}")
    endif ()

    # 4. 复制所有依赖的 Qt DLL（修正路径：从 QT_BIN_PATH 复制）
    set(QT_REQUIRED_LIBS
            Core        # 核心模块
            Gui         # 图形界面模块
            Widgets     # 控件模块
            Network     # 网络模块
            OpenGL      # OpenGL 模块
            SerialPort  # 串口模块
            Sql         # 数据库模块
            Svg         # SVG 图形模块
            WebChannel  # Web 通信模块
            WebSockets  # WebSocket 模块
            Multimedia  # 多媒体模块
            Concurrent  # 并发编程模块
            MultimediaWidgets  # 多媒体控件模块
    )
    foreach (QT_LIB ${QT_REQUIRED_LIBS})
        set(QT_DLL_PATH "${QT_BIN_PATH}/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll")
        if (EXISTS "${QT_DLL_PATH}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_DLL_PATH}"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                    COMMENT "Copying Qt DLL: Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
            )
        else ()
            message(FATAL_ERROR "Qt DLL missing! Path: ${QT_DLL_PATH}, please check if this component of Qt is installed.")
        endif ()
    endforeach ()

    # 5. 复制项目自定义 DLL 文件（修正：使用 file(GLOB) 获取文件列表）
    set(CUSTOM_DLL_PATH "${CMAKE_SOURCE_DIR}/lib/Debug")
    if (EXISTS "${CUSTOM_DLL_PATH}")
        # 获取所有 DLL 文件
        file(GLOB CUSTOM_DLLS "${CUSTOM_DLL_PATH}/*.dll")

        if (CUSTOM_DLLS)
            # 为每个 DLL 文件创建复制命令
            foreach (CUSTOM_DLL ${CUSTOM_DLLS})
                get_filename_component(DLL_NAME "${CUSTOM_DLL}" NAME)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${CUSTOM_DLL}"
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                        COMMENT "Copying custom DLL: ${DLL_NAME}"
                )
            endforeach ()
        else ()
            message(WARNING "No DLL files found in: ${CUSTOM_DLL_PATH}")
        endif ()
    else ()
        message(WARNING "Custom DLL path does not exist: ${CUSTOM_DLL_PATH}")
    endif ()

    # 6. 复制资源文件夹
    set(RESOURCE_FOLDERS
            "assets"
            "FrameworkShaders"
            "Resources"
            "SampleShaders"
    )

    foreach (FOLDER ${RESOURCE_FOLDERS})
        set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/${FOLDER}")
        set(TARGET_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${FOLDER}")

        if (EXISTS "${SOURCE_PATH}")
            # 递归复制整个文件夹
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${SOURCE_PATH}"
                    "${TARGET_PATH}"
                    COMMENT "Copying resource folder: ${FOLDER}"
            )
        else ()
            message(WARNING "Resource folder does not exist: ${SOURCE_PATH}")
        endif ()
    endforeach ()
endif ()