# 第三方依赖与资源信用

## C++ 依赖

| 库名               | 版本     | 描述                                                         | 许可协议            | 官方链接                          |
| :------------------: | :--------: | :------------------------------------------------------------: | :-------------------: | :---------------------------------: |
| Qt                 | 5.15.2   | 跨平台 C++ 应用程序开发框架，提供 UI、网络、数据库等功能。   | LGPLv3 / GPL        | <https://www.qt.io>               |
| GLEW               | 2.1.0    | OpenGL 扩展加载库，用于轻松访问现代 OpenGL 扩展功能。        | MIT                 | <http://glew.sourceforge.net>     |
| GLFW               | 3.4.0    | 跨平台库，用于创建窗口、处理输入和 OpenGL/OpenGL ES 上下文。 | MIT                 | <https://www.glfw.org>            |
| Live2D Cubism Core | SDK 内置 | Live2D 原生的 2D 角色实时动画引擎核心库。                    | 专有协议            | <https://www.live2d.com>          |
| stb_image          | 2.30     | 单头文件图像加载库，支持 PNG、JPEG、BMP 等格式。             | MIT / Public Domain | <https://github.com/nothings/stb> |
| stb_truetype       | 1.26     | 单头文件 TrueType 字体光栅化库。                             | MIT / Public Domain | <https://github.com/nothings/stb> |
| stb_image_resize2  | 2.12     | 单头文件高质量图像缩放库。                                   | MIT / Public Domain | <https://github.com/nothings/stb> |
| stb_ds             | 0.67     | 单头文件 C 语言动态数组和哈希表实现。                        | MIT / Public Domain | <https://github.com/nothings/stb> |

## Python 依赖

| 库名                      | 版本      | 描述                                                            | 许可协议         | 官方链接                                                   |
| :-------------------------: | :---------: | :---------------------------------------------------------------: | :----------------: | :----------------------------------------------------------: |
| aiohappyeyeballs          | 2.6.1     | 为 aiohttp 提供 Happy Eyeballs 连接加速算法。                   | MIT              | <https://pypi.org/project/aiohappyeyeballs>                |
| aiohttp                   | 3.13.5    | 异步 HTTP 客户端/服务器框架，支持 WebSocket。                   | Apache-2.0       | <https://docs.aiohttp.org>                                 |
| aiosignal                 | 1.4.0     | 异步信号分发实现，辅助 aiohttp 等异步库的事件通知。             | MIT              | <https://pypi.org/project/aiosignal>                       |
| altgraph                  | 0.17.5    | 用于分析和可视化 Python 程序依赖图的工具。                      | MIT              | <https://pypi.org/project/altgraph>                        |
| attrs                     | 26.1.0    | 通过装饰器简化 Python 类的定义，减少样板代码。                  | MIT              | <https://www.attrs.org>                                    |
| blinker                   | 1.9.0     | 快速的、简单的信号/事件分发系统，用于松耦合通信。               | MIT              | <https://pypi.org/project/blinker>                         |
| certifi                   | 2026.4.22 | 提供 Mozilla 的 CA 证书库，用于 TLS 验证。                      | MPL-2.0          | <https://pypi.org/project/certifi>                         |
| cffi                      | 2.0.0     | C 语言外部函数接口，允许 Python 代码调用 C 库。                 | MIT              | <https://pypi.org/project/cffi>                            |
| click                     | 8.3.3     | Python 命令行界面开发工具，支持嵌套命令和自动生成帮助。         | BSD-3-Clause     | <https://palletsprojects.com/p/click/>                     |
| colorama                  | 0.4.6     | 跨平台的终端彩色输出工具，使 ANSI 颜色码在 Windows 下也能工作。 | BSD-3-Clause     | <https://pypi.org/project/colorama>                        |
| edge-tts                  | 7.2.8     | 使用 Microsoft Edge 的文本转语音服务，提供命令行和 Python API。 | GPL-3.0          | <https://github.com/rany2/edge-tts>                        |
| emoji                     | 2.15.0    | 在 Python 中处理 emoji 字符，支持搜索、替换和语义识别。         | BSD-3-Clause     | <https://pypi.org/project/emoji>                           |
| Flask                     | 3.1.3     | 轻量级 WSGI Web 应用框架，易于扩展。                            | BSD-3-Clause     | <https://flask.palletsprojects.com>                        |
| frozenlist                | 1.8.0     | 不可变列表，通常在 aiohttp 及其生态系统中使用。                 | Apache-2.0       | <https://pypi.org/project/frozenlist>                      |
| gevent                    | 26.4.0    | 基于 greenlet 的协程并发网络库，提供高层次的同步 API。          | MIT              | <http://www.gevent.org>                                    |
| greenlet                  | 3.4.0     | 轻量级的“微线程”实现，允许多个执行单元之间切换。                | MIT              | <https://pypi.org/project/greenlet>                        |
| idna                      | 3.13      | 实现国际化域名（IDNA）2008 协议的编码解码。                     | BSD-3-Clause     | <https://pypi.org/project/idna>                            |
| itsdangerous              | 2.2.0     | 数据签名与序列化，用于在不可信环境中安全传输数据。              | BSD-3-Clause     | <https://pypi.org/project/itsdangerous>                    |
| Jinja2                    | 3.1.6     | 快速、现代、设计友好的 Python 模板引擎。                        | BSD-3-Clause     | <https://palletsprojects.com/p/jinja/>                     |
| MarkupSafe                | 3.0.3     | 将字符串安全地转义为 HTML/XML，防止注入攻击。                   | BSD-3-Clause     | <https://pypi.org/project/MarkupSafe>                      |
| multidict                 | 6.7.1     | 支持多键值对的字典实现，常用于 HTTP 头处理。                    | Apache-2.0       | <https://pypi.org/project/multidict>                       |
| packaging                 | 26.2      | 提供 Python 包版本管理与依赖解析的实用工具。                    | Apache-2.0 / BSD | <https://pypi.org/project/packaging>                       |
| pefile                    | 2024.8.26 | 解析 PE 文件格式（Windows 可执行文件）的纯 Python 库。          | MIT              | <https://github.com/erocarrera/pefile>                     |
| propcache                 | 0.4.1     | 属性缓存工具，用于 aiohttp 生态中加速重复属性访问。             | Apache-2.0       | <https://pypi.org/project/propcache>                       |
| pycparser                 | 3.0       | 纯 Python 实现 C 语言语法解析器，用于分析 C 代码。              | BSD-3-Clause     | <https://pypi.org/project/pycparser>                       |
| pyinstaller               | 6.20.0    | 将 Python 程序及应用打包为独立可执行文件。                      | GPL              | <https://www.pyinstaller.org>                              |
| pyinstaller-hooks-contrib | 2026.4    | PyInstaller 的社区贡献钩子，增强对第三方库的打包支持。          | GPL              | <https://github.com/pyinstaller/pyinstaller-hooks-contrib> |
| PySide6                   | 6.11.0    | Qt for Python 的主要绑定，提供全面的 Qt 6 API 访问。            | LGPL             | <https://doc.qt.io/qtforpython/>                           |
| PySide6_Addons            | 6.11.0    | PySide6 附加模块，包含 Qt 的额外工具和扩展。                    | LGPL             | <https://doc.qt.io/qtforpython/>                           |
| PySide6_Essentials        | 6.11.0    | PySide6 核心模块，包含 Qt 基础功能所需的部分。                  | LGPL             | <https://doc.qt.io/qtforpython/>                           |
| python-dotenv             | 1.2.2     | 从 .env 文件加载键值对环境变量到 os.environ。                   | BSD-3-Clause     | <https://pypi.org/project/python-dotenv>                   |
| pywin32-ctypes            | 0.2.3     | 通过 ctypes 访问 Windows API 的轻量级实现。                     | BSD              | <https://pypi.org/project/pywin32-ctypes>                  |
| shiboken6                 | 6.11.0    | PySide6 的底层 Python/C++ 绑定生成器和运行时。                  | LGPL             | <https://doc.qt.io/qtforpython/>                           |
| tabulate                  | 0.10.0    | 美观地格式化输出表格数据，支持多种输出格式。                    | MIT              | <https://pypi.org/project/tabulate>                        |
| typing_extensions         | 4.15.0    | 提供 Python 标准库 typing 模块的实验性类型提示。                | Python-2.0       | <https://pypi.org/project/typing-extensions>               |
| websocket-client          | 1.9.0     | 纯 Python 实现的 WebSocket 客户端。                             | LGPL             | <https://github.com/websocket-client/websocket-client>     |
| Werkzeug                  | 3.1.8     | 综合性的 WSGI 工具库，为 Web 框架提供底层支持。                 | BSD-3-Clause     | <https://werkzeug.palletsprojects.com>                     |
| yarl                      | 1.23.0    | 带引用和解析功能的 URL 库，常用于异步网络栈。                   | Apache-2.0       | <https://pypi.org/project/yarl>                            |
| zope.event                | 6.1       | 简单的事件发布/订阅机制。                                       | ZPL-2.1          | <https://pypi.org/project/zope.event>                      |
| zope.interface            | 8.4       | 接口定义与实现检查框架，用于组件化编程。                        | ZPL-2.1          | <https://pypi.org/project/zope.interface>                  |

## 其他使用的资源

| 资源名称                   | 说明                                                          | 来源链接                                             |
| :--------------------------: | :-------------------------------------------------------------: | :----------------------------------------------------: |
| IconaMoon                  | 项目中使用的图标集。                                          | <https://github.com/dariushhpg1/IconaMoon>           |
| Maple Font                 | 项目中使用的开源字体。                                        | <https://github.com/subframe7536/Maple-font>         |
| Live2D Cubism SDK 附加资源 | 包含 SDK 提供的示例模型、动作及演示数据，受各自版权条款保护。 | <https://www.live2d.com/zh-CHS/sdk/download/native/> |
