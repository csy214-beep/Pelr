# ============================================================================
# 项目基本信息
# ============================================================================
cmake_minimum_required(VERSION 3.3)
project(Pelr)

# 配置选项
set(D_OR_R OFF)                # ON: Debug, OFF: Release
set(CONSOLE OFF)               # ON: 显示控制台, OFF: 隐藏控制台（GUI模式）

# 平台与标准设置
if (WIN32)
    add_compile_definitions(UNICODE _UNICODE)   # Windows Unicode 支持
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)        # 自动处理 Qt MOC
set(CMAKE_AUTORCC ON)        # 自动处理 Qt RCC
set(CMAKE_AUTOUIC ON)        # 自动处理 Qt UIC

# Qt 路径与模块查找
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/mingw81_64")
find_package(Qt5 COMPONENTS
        Core Gui Widgets Network OpenGL SerialPort Sql Svg
        WebChannel WebSockets Multimedia Concurrent MultimediaWidgets
        LinguistTools
        REQUIRED)

# ============================================================================
# 资源与翻译文件
# ============================================================================

# 资源文件（.qrc）
qt5_add_resources(QT_RESOURCES Resource.qrc)

# 翻译文件（.ts）
set(TRANSLATION_FILES
        translations/language_zh_CN.ts
        translations/language_en_US.ts
)

# ============================================================================
# 源文件收集（使用 GLOB，注意新文件需手动重新运行 CMake）
# ============================================================================

# 设置 Live2D 框架渲染方式（必须）
set(FRAMEWORK_SOURCE OpenGL)

# Live2D SDK 源文件
file(GLOB LAPP_2D_SRC "LAppLive2D/src/*.cpp")

# 各功能模块源文件
file(GLOB AI_SRC "src/ai/*.cpp" "src/ai/*.h" "src/ai/*.hpp")
file(GLOB CORE_SRC "src/core/*.cpp" "src/core/*.h" "src/core/*.hpp")
file(GLOB KEYBOARD_SRC "src/keyboard/*.cpp" "src/keyboard/*.h" "src/keyboard/*.hpp")
file(GLOB MODEL_SRC "src/model/*.cpp" "src/model/*.h" "src/model/*.hpp")
file(GLOB TRANSLATION_SRC "src/translation/*.cpp" "src/translation/*.h" "src/translation/*.hpp")
file(GLOB TTS_SRC "src/tts/*.cpp" "src/tts/*.h" "src/tts/*.hpp")
file(GLOB UTILS_SRC "src/utils/*.cpp" "src/utils/*.h" "src/utils/*.hpp")
file(GLOB UI_SRC "src/ui/*.cpp" "src/ui/*.h" "src/ui/*.hpp")
file(GLOB UI_FILES "src/ui/*.ui")                     # .ui 文件单独收集

# 合并所有源文件（.cpp），供编译使用
set(ALL_CPP_SOURCES
        ${LAPP_2D_SRC}
        ${AI_SRC} ${CORE_SRC} ${KEYBOARD_SRC} ${MODEL_SRC}
        ${TRANSLATION_SRC} ${TTS_SRC} ${UI_SRC} ${UTILS_SRC}
        "src/main.cpp"
)

# 合并所有头文件（.h/.hpp），供 MOC 处理
set(ALL_HEADER_FILES
        ${AI_SRC} ${CORE_SRC} ${KEYBOARD_SRC} ${MODEL_SRC}
        ${TRANSLATION_SRC} ${TTS_SRC} ${UI_SRC} ${UTILS_SRC}
)

# 注意：${ALL_CPP_SOURCES} 只包含 .cpp，${ALL_HEADER_FILES} 包含 .h/.hpp
# 后续通过 qt5_wrap_cpp 从 ALL_HEADER_FILES 生成 MOC 文件

# ============================================================================
# Qt 自动化工具处理
# ============================================================================

# MOC：处理头文件中的 Q_OBJECT 宏
qt5_wrap_cpp(MOC_SOURCES ${ALL_HEADER_FILES})

# UIC：将 .ui 文件转换为 C++ 头文件
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# 生成翻译文件（.qm），扫描所有源文件、头文件、UI 文件中的 tr()
qt5_create_translation(QM_FILES
        ${ALL_CPP_SOURCES} ${ALL_HEADER_FILES} ${UI_FILES}
        ${TRANSLATION_FILES}
)

# kissfft 库
add_library(kissfft STATIC
    thirdParty/kissfft/kiss_fft.c
    thirdParty/kissfft/kiss_fftr.c
)
target_include_directories(kissfft PUBLIC thirdParty/kissfft)

# ============================================================================
# 头文件包含路径
# ============================================================================
include_directories(
        ./
        src/ai
        src/core
        src/keyboard
        src/model
        src/translation
        src/tts
        src/ui
        src/utils
        LAppLive2D/inc
        thirdParty/Core/include
        thirdParty/Framework/src
        thirdParty/glew/include
        thirdParty/glew/include/GL
        thirdParty/glfw/include
        thirdParty/glfw/include/GLFW
        thirdParty/stb
        thirdParty/kissfft
        ${CMAKE_CURRENT_BINARY_DIR}      # 存放生成的 ui_*.h 文件
)

# 指定 UI 文件搜索路径（AUTOUIC 需要）
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src/ui")

# ============================================================================
# 链接库路径
# ============================================================================
link_directories("${CMAKE_SOURCE_DIR}/lib/Debug")

# ============================================================================
# 可执行文件
# ============================================================================

# 根据 CONSOLE 选项决定是否显示控制台
if (NOT CONSOLE)
    set(CON WIN32)
endif ()

add_executable(${PROJECT_NAME}
        ${CON}                     # WIN32 或空
        ${ALL_CPP_SOURCES}         # 所有 .cpp 源文件
        ${MOC_SOURCES}             # MOC 生成的文件
        ${UI_HEADERS}              # UI 生成的头文件
        ${QT_RESOURCES}            # RCC 生成的资源文件
        ${QM_FILES}                # 翻译文件（.qm）
)

# ============================================================================
# 链接库
# ============================================================================
target_link_libraries(${PROJECT_NAME}
        Framework
        libglew32d
        glfw3
        Live2DCubismCore_d
        opengl32
        glu32
        kissfft
        ksuser
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::OpenGL
        Qt5::SerialPort
        Qt5::Svg
        Qt5::WebChannel
        Qt5::WebSockets
        Qt5::Multimedia
        Qt5::Concurrent
        Qt5::MultimediaWidgets
)

# ============================================================================
# Windows 平台后处理
# ============================================================================
if (WIN32)
    # 程序图标
    set(APP_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/image/Pelr.ico")
    set(APP_RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/icon.rc")
    if (EXISTS ${APP_ICON_FILE} AND EXISTS ${APP_RC_FILE})
        target_sources(${PROJECT_NAME} PRIVATE ${APP_RC_FILE})
        # 根据 CONSOLE 设置是否显示控制台
        if (NOT CONSOLE)
            set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
        else ()
            set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
        endif ()
    else ()
        message(WARNING "Program icon file or RC script is missing, default icon will be used!")
    endif ()

    # 复制翻译文件（.qm）到输出目录
    set(QM_OUTPUT_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/translations")
    foreach (qm_file ${QM_FILES})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy ${qm_file} ${QM_OUTPUT_DIR}/
                COMMENT "Copying translation file: ${qm_file}")
    endforeach ()

    # 复制 Qt 运行时依赖（DLL 和插件）
    if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(DEBUG_SUFFIX "")          # 使用 Release 后缀，如需 Debug 可改为 "d"
        set(QT_BIN_PATH "${CMAKE_PREFIX_PATH}/bin")
        set(QT_PLUGIN_PATH "${CMAKE_PREFIX_PATH}/plugins")

        if (NOT EXISTS ${QT_BIN_PATH})
            message(FATAL_ERROR "Qt bin directory does not exist: ${QT_BIN_PATH}")
        endif ()
        if (NOT EXISTS ${QT_PLUGIN_PATH})
            message(FATAL_ERROR "Qt plugins directory does not exist: ${QT_PLUGIN_PATH}")
        endif ()

        # 复制平台插件
        set(QT_WINDOWS_PLUGIN "${QT_PLUGIN_PATH}/platforms/qwindows${DEBUG_SUFFIX}.dll")
        if (EXISTS ${QT_WINDOWS_PLUGIN})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                    COMMAND ${CMAKE_COMMAND} -E copy "${QT_WINDOWS_PLUGIN}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
                    COMMENT "Copying Qt platform plugin: qwindows${DEBUG_SUFFIX}.dll")
        else ()
            message(FATAL_ERROR "Qt platform plugin missing: ${QT_WINDOWS_PLUGIN}")
        endif ()

        # 复制 mediaservice 插件（多媒体）
        set(QT_MEDIASERVICE_PLUGIN_PATH "${QT_PLUGIN_PATH}/mediaservice")
        if (EXISTS ${QT_MEDIASERVICE_PLUGIN_PATH})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/mediaservice/"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${QT_MEDIASERVICE_PLUGIN_PATH}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/mediaservice/"
                    COMMENT "Copying Qt mediaservice plugins")
        else ()
            message(WARNING "Qt mediaservice plugins directory does not exist: ${QT_MEDIASERVICE_PLUGIN_PATH}")
        endif ()

        # 复制所需 Qt DLL
        set(QT_REQUIRED_LIBS
                Core Gui Widgets Network OpenGL SerialPort Sql Svg
                WebChannel WebSockets Multimedia Concurrent MultimediaWidgets)
        foreach (QT_LIB ${QT_REQUIRED_LIBS})
            set(QT_DLL_PATH "${QT_BIN_PATH}/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll")
            if (EXISTS ${QT_DLL_PATH})
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy "${QT_DLL_PATH}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                        COMMENT "Copying Qt DLL: Qt5${QT_LIB}${DEBUG_SUFFIX}.dll")
            else ()
                message(FATAL_ERROR "Qt DLL missing: ${QT_DLL_PATH}")
            endif ()
        endforeach ()

        # 复制自定义 DLL（项目依赖）
        set(CUSTOM_DLL_PATH "${CMAKE_SOURCE_DIR}/lib/Debug")
        if (EXISTS ${CUSTOM_DLL_PATH})
            file(GLOB CUSTOM_DLLS "${CUSTOM_DLL_PATH}/*.dll")
            foreach (CUSTOM_DLL ${CUSTOM_DLLS})
                get_filename_component(DLL_NAME "${CUSTOM_DLL}" NAME)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy "${CUSTOM_DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                        COMMENT "Copying custom DLL: ${DLL_NAME}")
            endforeach ()
        else ()
            message(WARNING "Custom DLL path does not exist: ${CUSTOM_DLL_PATH}")
        endif ()
    endif ()

    # 复制资源文件夹
    set(RESOURCE_FOLDERS
            "assets"
            "FrameworkShaders"
            "Resources"
            "SampleShaders"
    )
    foreach (FOLDER ${RESOURCE_FOLDERS})
        set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/${FOLDER}")
        set(TARGET_PATH "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${FOLDER}")
        if (EXISTS ${SOURCE_PATH})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_PATH}" "${TARGET_PATH}"
                    COMMENT "Copying resource folder: ${FOLDER}")
        else ()
            message(WARNING "Resource folder does not exist: ${SOURCE_PATH}")
        endif ()
    endforeach ()
endif ()