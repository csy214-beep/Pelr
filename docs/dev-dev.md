## 开发指引

这个文件很简单，可能需要大家共同完善。

### Python

<https://github.com/csy214-beep/Pelr_tts_tr>

自`20260503.14`起，python tts server已独立出当前项目，可以选择性配置。

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

在打包发布前，需要确保`CMakeLists.txt`的这里配置为Release: `OFF`

```txt
# 配置选项
# ====================== 全局配置开关 ======================
set(DEBUG_MODE OFF)           # ON: Debug, OFF: Release
```

---

### Python 包管理

<https://github.com/csy214-beep/Pelr_tts_tr>

```shell
pip install pip-review
# 查看可更新的包
pip-review
# 自动更新所有包
pip-review --auto
```

导出所有包（不推荐）

```shell
pip freeze > requirements.txt
```

导出依赖包到requirements.txt

```shell
pip install pigar
pigar generate
```
