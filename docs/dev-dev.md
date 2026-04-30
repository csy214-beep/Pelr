## 开发指引

这个文件很简单，可能需要大家共同完善。

### Python

使用Python 3.10 创建虚拟环境，并安装依赖，在根目录执行：

```bash
python -m venv .venv
call .venv\Scripts\activate
pip install -r requirements.txt
```

运行程序：

```bash
python tts_server\tts_server.py
```

打包：

```bash
pyinstaller -w tts_server\tts_server.py

```

在 `dist` 目录可以找到构建产物，我们需要的是`tts_server`文件夹里面的所有内容。

此时可以新建一个 `release` 目录或者 `a` 目录，把`tts_server`文件夹里面的所有内容移动/复制到`release`目录或者 `a` 目录。

### C++

需要确认项目资源是否配备完成：

- `FrameworkShaders`
- `assets`
- `lib`
- `Resources`
- `SampleShaders`
- `thirdParty/`
  - `Core`
  - `Framework`
  - `glew`
  - `stb`

---

在打包发布前，需要确保`CMakeLists.txt`的这两个选项为 OFF

```txt
# 配置选项
set(D_OR_R OFF)                # ON: Debug, OFF: Release
set(CONSOLE OFF)               # ON: 显示控制台, OFF: 隐藏控制台（GUI模式）
```

---

src\main.cpp：

确保第30行为`#define DEBUG false`，以禁用debug模式，使得日志系统可以将日志写入文件。

### Python 包管理

```shell
pip install pip-review
# 查看可更新的包
pip-review
# 自动更新所有包
pip-review --auto
```

导出所有包

```shell
pip freeze > requirements.txt
```

似乎可以导出依赖包

```shell
pip install pigar
pigar generate
```
