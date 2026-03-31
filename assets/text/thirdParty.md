本项目使用了很多第三方库，详情请移步[WiKi](https://gitee.com/Pfolg/Pelr/wikis)了解。

#### [Cpp 工具链](https://gitee.com/Pfolg/Pelr/wikis/Cpp-%E5%B7%A5%E5%85%B7%E9%93%BE)

| 分类        | 库名称                | 版本     | 用途说明                    | 许可协议              | 官方来源                                                       |
|-----------|--------------------|--------|-------------------------|-------------------|------------------------------------------------------------|
| GUI框架     | Qt                 | 5.15.2 | 跨平台应用开发(UI/网络/数据库/多媒体等) | LGPLv3/GPL        | [qt.io](https://www.qt.io)                                 |
| 图形渲染      | GLEW               | 2.1.0  | OpenGL扩展加载和管理           | MIT               | [glew.sourceforge.net](http://glew.sourceforge.net/)       |
| 图形渲染      | GLFW               | 3.4.0  | 窗口和上下文管理                | MIT               | [glfw.org](https://www.glfw.org/)                          |
| 2D动画引擎    | Live2D Cubism Core | SDK内置  | 二次元角色动画渲染               | 专有协议              | [live2d.com](https://www.live2d.com)                       |
| 图像处理      | stb_image          | 2.30   | 多格式图像加载(PNG/JPG/BMP等)   | MIT/Public Domain | [github.com/nothings/stb](https://github.com/nothings/stb) |
| 图像处理      | stb_truetype       | 1.26   | TrueType字体光栅化           | MIT/Public Domain | [github.com/nothings/stb](https://github.com/nothings/stb) |
| 图像处理      | stb_image_resize2  | 2.12   | 高质量图像缩放                 | MIT/Public Domain | [github.com/nothings/stb](https://github.com/nothings/stb) |
| 数据结构      | stb_ds             | 0.67   | C语言动态数组和哈希表             | MIT/Public Domain | [github.com/nothings/stb](https://github.com/nothings/stb) |
| 打包工具      | PyInstaller        | 6.15.0 | 应用打包和分发                 | GPL               | [pyinstaller.org](https://www.pyinstaller.org/)            |
| Windows支持 | pywin32-ctypes     | 0.2.3  | Windows API调用支持         | BSD               | [pypi.org](https://pypi.org/project/pywin32-ctypes/)       |

#### [Python 工具链](https://gitee.com/Pfolg/Pelr/wikis/Python-%E5%B7%A5%E5%85%B7%E9%93%BE)

| **分类**         | **库名**                    | **版本**    | **用途**                                             | **授权协议**       | **来源**                                                             |
|----------------|---------------------------|-----------|----------------------------------------------------|----------------|--------------------------------------------------------------------|
| **打包工具**       | PyInstaller               | 6.18.0    | 应用打包与分发                                            | GPL            | [PyInstaller](https://www.pyinstaller.org)                         |
|                | pyinstaller-hooks-contrib | 2026.0    | PyInstaller 附加钩子支持                                 | GPL            | [GitHub](https://github.com/pyinstaller/pyinstaller-hooks-contrib) |
| **依赖分析**       | altgraph                  | 0.17.5    | 图形化依赖分析工具                                          | MIT            | [PyPI](https://pypi.org/project/altgraph/)                         |
| **Windows工具链** | pywin32-ctypes            | 0.2.3     | Windows API 调用                                     | BSD            | [PyPI](https://pypi.org/project/pywin32-ctypes)                    |
|                | pefile                    | 2024.8.26 | PE文件格式解析                                           | MIT            | [GitHub](https://github.com/erocarrera/pefile)                     |
| **网络通信**       | websocket-client          | 1.9.0     | WebSocket 客户端实现                                    | LGPL           | [GitHub](https://github.com/websocket-client/websocket-client)     |
|                | aiohttp                   | 3.13.4    | 异步HTTP客户端/服务器                                      | Apache-2.0     | [aiohttp](https://docs.aiohttp.org)                                |
|                | aiohappyeyeballs          | 2.6.1     | 异步连接加速                                             | MIT            | [PyPI](https://pypi.org/project/aiohappyeyeballs)                  |
|                | aiosignal                 | 1.4.0     | 异步信号分发                                             | MIT            | [PyPI](https://pypi.org/project/aiosignal)                         |
|                | frozenlist                | 1.8.0     | 不可变列表                                              | Apache-2.0     | [PyPI](https://pypi.org/project/frozenlist)                        |
|                | multidict                 | 6.7.1     | 多键值字典                                              | Apache-2.0     | [PyPI](https://pypi.org/project/multidict)                         |
|                | yarl                      | 1.23.0    | URL解析                                              | Apache-2.0     | [PyPI](https://pypi.org/project/yarl)                              |
|                | propcache                 | 0.4.1     | 属性缓存                                               | Apache-2.0     | [PyPI](https://pypi.org/project/propcache)                         |
| **工具辅助**       | packaging                 | 26.0      | 包版本与依赖管理                                           | Apache-2.0/BSD | [PyPI](https://pypi.org/project/packaging/)                        |
|                | python-dotenv             | 1.2.2     | 环境变量加载                                             | BSD-3-Clause   | [PyPI](https://pypi.org/project/python-dotenv)                     |
|                | tabulate                  | 0.10.0    | 表格输出                                               | MIT            | [PyPI](https://pypi.org/project/tabulate)                          |
|                | typing_extensions         | 4.15.0    | 类型注解扩展                                             | Python-2.0     | [PyPI](https://pypi.org/project/typing-extensions)                 |
|                | emoji                     | 2.15.0    | Emoji处理                                            | BSD-3-Clause   | [PyPI](https://pypi.org/project/emoji)                             |
|                | colorama                  | 0.4.6     | 终端彩色输出                                             | BSD-3-Clause   | [PyPI](https://pypi.org/project/colorama)                          |
|                | certifi                   | 2026.2.25 | CA证书集                                              | MPL-2.0        | [PyPI](https://pypi.org/project/certifi)                           |
|                | idna                      | 3.11      | 国际化域名处理                                            | BSD-3-Clause   | [PyPI](https://pypi.org/project/idna)                              |
|                | attrs                     | 26.1.0    | 简化类定义                                              | MIT            | [PyPI](https://pypi.org/project/attrs)                             |
|                | cffi                      | 2.0.0     | C函数接口                                              | MIT            | [PyPI](https://pypi.org/project/cffi)                              |
|                | pycparser                 | 3.0       | C语法解析                                              | BSD-3-Clause   | [PyPI](https://pypi.org/project/pycparser)                         |
| **GUI框架**      | PySide6                   | 6.10.1    | Qt for Python 跨平台GUI框架                             | LGPL           | [Qt for Python](https://doc.qt.io/qt-6/pyside6-index.html)         |
|                | PySide6_Addons            | 6.10.1    | PySide6 附加模块                                       | LGPL           | [Qt for Python](https://doc.qt.io/qt-6/pyside6-index.html)         |
|                | PySide6_Essentials        | 6.10.1    | PySide6 核心模块                                       | LGPL           | [Qt for Python](https://doc.qt.io/qt-6/pyside6-index.html)         |
| **绑定工具**       | shiboken6                 | 6.10.1    | Python/C++ 绑定生成器                                   | LGPL           | [Qt for Python](https://doc.qt.io/qt-6/pyside6-index.html)         |
| **Web框架**      | Flask                     | 3.1.3     | 轻量级Web框架                                           | BSD-3-Clause   | [Flask](https://flask.palletsprojects.com)                         |
|                | Werkzeug                  | 3.1.7     | WSGI工具库                                            | BSD-3-Clause   | [Werkzeug](https://werkzeug.palletsprojects.com)                   |
|                | Jinja2                    | 3.1.6     | 模板引擎                                               | BSD-3-Clause   | [Jinja2](https://palletsprojects.com/p/jinja/)                     |
|                | MarkupSafe                | 3.0.3     | 字符串转义                                              | BSD-3-Clause   | [PyPI](https://pypi.org/project/MarkupSafe)                        |
|                | itsdangerous              | 2.2.0     | 数据签名                                               | BSD-3-Clause   | [PyPI](https://pypi.org/project/itsdangerous)                      |
|                | blinker                   | 1.9.0     | 信号通知                                               | MIT            | [PyPI](https://pypi.org/project/blinker)                           |
|                | click                     | 8.3.1     | 命令行界面                                              | BSD-3-Clause   | [PyPI](https://pypi.org/project/click)                             |
| **语音合成**       | edge-tts                  | 7.2.8     | 微软Edge TTS服务                                       | GPL-3.0        | [GitHub](https://github.com/rany2/edge-tts)                        |
|                | openai-edge-tts           | 2.0.0     | local, OpenAI-compatible  (TTS) API using edge-tts | GPL-3.0        | [GitHub](https://github.com/travisvn/openai-edge-tts)              |
| **并发工具**       | gevent                    | 25.9.1    | 协程并发网络库                                            | MIT            | [gevent](http://www.gevent.org)                                    |
|                | greenlet                  | 3.3.2     | 协程上下文切换                                            | MIT            | [PyPI](https://pypi.org/project/greenlet)                          |
|                | zope.event                | 6.1       | 事件发布订阅                                             | ZPL-2.1        | [PyPI](https://pypi.org/project/zope.event)                        |
|                | zope.interface            | 8.2       | 接口定义                                               | ZPL-2.1        | [PyPI](https://pypi.org/project/zope.interface)                    |

## 其他资源

图标资源：

[dariushhpg1/IconaMoon - GitHub](https://github.com/dariushhpg1/IconaMoon)

使用字体：

[subframe7536/Maple-font - GitHub](https://github.com/subframe7536/Maple-font)

使用了[Cubism SDK For Native](https://www.live2d.com/zh-CHS/sdk/download/native/)的附加资源（Live2D模型、Demo等）。

演示资源恕不提供，大多数模型资源有版权限制，请自行搜索下载。

---

## 致谢

感谢开源社区，感谢我所遇到的所有善良之人，感谢你们给予的帮助。

+ [Live2D Cubism](https://www.live2d.com/zh-CHS/)
+ [【live2d教程】从零开始做模型吧 合集](https://www.bilibili.com/video/BV1c4411s7NR/)
+ [Live2D】画手向Live2D模型教程，30min学会Live2D！](https://www.bilibili.com/video/BV1PA411i7f3/)
+ [Live2D For C++(Qt)实现二次元桌宠 by 見崎音羽](https://www.bilibili.com/video/BV1TtkHYpEDA/)
+ [見崎音羽](https://space.bilibili.com/140315806)
+ [Hucci写代码](https://space.bilibili.com/1318868)
+ [How to Build a Qt Application Using Visual Studio Code](https://www.youtube.com/watch?v=j2Gfkrajb1M)
+ [madhawa polkotuwa](https://github.com/madhawapolkotuwa)
+ [黑马程序员匠心之作|C++教程从0到1入门编程,学习编程不再难](https://www.bilibili.com/video/BV1et411b73Z)
+ [Python全栈开发教程](https://www.bilibili.com/video/BV1wD4y1o7AS/)
+ [EasyLive2D/live2d-py](https://github.com/EasyLive2D/live2d-py)

还有很多很多人，在我的求索之路上，他们都给予了我帮助，恕我无法一一列举。

---

## 同类项目

以下项目在一些方面给予了我很多思考，在此表示感谢。

[moeru-ai/airi - GitHub](https://github.com/moeru-ai/airi)

[Open-LLM-VTuber/Open-LLM-VTuber - GitHub](https://github.com/Open-LLM-VTuber/Open-LLM-VTuber)

[FLCYR/QtLive2dDesktop - GitHub](https://github.com/FLCYR/QtLive2dDesktop)

[LorisYounger/VPet - GitHub](https://github.com/LorisYounger/VPet)

[ANDYERE999/DesktopGirlClient - GitHub](https://github.com/ANDYERE999/DesktopGirlClient)

[Arkueid/Live2DDesktop-CMake - GitHub](https://github.com/Arkueid/Live2DDesktop-CMake)
