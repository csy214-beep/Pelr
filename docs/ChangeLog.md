<!-- > [!NOTE]
> GitHub: <https://github.com/igugyj/Pelr>
>
> Gitee: <https://gitee.com/Pfolg/Pelr>
>
> SourceForge: <https://sourceforge.net/projects/pfolg-plauncher> -->

[//]: # "[![Release History](https://img.shields.io/badge/-Release_History-blue?logo=readthedocs&logoColor=white)](https://github.com/igugyj/Pelr/blob/master/docs/ChangeLog.md)"

## v20260503.14-dev

1. docs: optimize docs
1. chore: rm old ver lapp code
1. docs: docs/dev-init.md
1. refactor: 2 submodule added & rm origin bad structure of thirdpart
1. chore: notice optimize
1. docs: simple outline
1. build: cmakeList.txt optimize
1. fix: voicevox test
1. docs: CREDITS.md

## v20260503.13.5 - 2026-05-03

### 更新内容

- **文档更新**：更新用户文档，提升使用指引的准确性与完整性。
- **TTS 设置页面**：优化页面显示逻辑，改善界面交互体验。
- **VoiceVox 模型**：优化模型加载时 StyleID 的载入逻辑，提高稳定性和正确性。
- **翻译功能**：新增腾讯云翻译能力，并补充相关配置说明与文档支持。
- **翻译功能**：libretranslate 简单支持

---
**Full Changelog**: <https://github.com/igugyj/Pelr/compare/20260405.12.18...20260503.13.5>

## v20260405.12.18 - 2026-05-02

> [!CAUTION]
> **不兼容变更**
>
> - `configData.dat` 基本配置文件不兼容，需重新配置

### ✨ 新特性

- **思考气泡动画**：增加对话过程中的思考效果，交互更自然。
- **VoiceVox TTS 引擎支持**：新增高质量语音合成后端，提供更多音色选择。
- **翻译后 TTS**：可将输出文本先翻译为指定语言，再通过 TTS 朗读（Qt 界面配置，Python 端实现）。
- **自定义 AI 接口（OpenAI 兼容）**：允许用户接入自己的 API 地址和密钥，不再局限于内置服务。
- **多 API 更新检查**：支持从多个备用源检测新版本，提高更新可靠性。
- **通知提示音**：新增系统通知时的可选提示音。

### 🔧 重构与优化

- **资源结构压缩**：优化资源打包方式，显著减小构建产物体积。
- **全新 QSS 界面样式**：引入 QSS 库重写样式表，视觉效果和一致性提升。
- **启动菜单布局优化**：调整菜单间距与位置，减少误触概率。
- **托盘样式逻辑重构**：规范托盘图标的激活、隐藏及消息处理逻辑。
- **设置页面 TTS 选项卡**：重新排版 TTS 相关设置项，操作更清晰。
- **About 对话框**：将 Markdown 文本浏览器设为自动打开外部链接。

### 🐛 问题修复

- **日志 UTF‑8 支持**：修复日志文件在 Windows 下编码乱码的问题。
- **默认气泡颜色**：修正初始对话气泡的颜色显示异常。

### 📄 文档与仓库

- 更新 `CREDITS.md` 与 `THANKS.md`，新增译者名单（translators==6.0.4）。
- 移除 Wiki，将内容合并入主文档目录。
- 添加 Issue 与 PR 模板，规范社区贡献流程。
- 仓库地址从 Gitee 迁移至 GitHub，同步更新相关链接与说明。

### 🔗 依赖项变动

- 引入 `kissfft` 替代原有的系统音频特征提取库，提升跨平台稳定性。

### 🛠 开发环境

- 开发工具由 CLion 替换为 VSCode，相关配置更新。

---

**Full Changelog**: <https://github.com/igugyj/Pelr/compare/20260219.11.17...20260405.12.18>

## v20260219.11.17 - 2026-04-01

> [!CAUTION]
> **不兼容变更**
>
> - `configData.dat` 基本配置文件不兼容，需重新配置
> - `ttsConfig.json` 虽兼容但建议重新配置以确保最佳体验

### ✨ 新增

- 音乐托盘支持颜色/符号自定义设置，优化气泡颜色逻辑
- 新增 `openai_edge_tts` 语音合成引擎
- 托盘菜单增加“打开用户文件夹”选项
- TTS 连接失败时自动尝试启动服务端，并增加相关配置

### 🔧 优化

- 控制界面适配响应式布局
- 重构语句文件模块，提升气泡消息稳定性
- 重构按键监听功能，优化按键提示体验
- 窗口显示时尝试提升至最前
- 启动线程弹窗标题及检查更新弹窗逻辑优化

### 🐛 修复

- 修复启动时意外显示计划外时间气泡的问题
- 修复按键标签中符号与键间缺少空格的问题
- README 错别字修正

### 📁 重构

- 项目重命名为 **Pelr**（原 PLauncher）
- 重构 src 目录结构

**完整的更新日志**: [查看详情](https://gitee.com/Pfolg/Pelr/compare/20260107.10.16...20260219.11.17)

## 20260107.10.16

### 更新内容

1. fix: 修复日志信息中的语言不一致问题
1. feat(core): 添加批量启动功能
1. fix(setting): 添加重置设置前的确认消息
1. feat(ui): 添加TTS和OpenWeather配置结构及读写方法(json)
1. feat: 如果无启动项则不显示菜单(第一次添加需重启)
1. feat: 如果系统启动超过20分钟则不运行star线程
1. feat: 添加通知弹窗功能并替换托盘消息
1. refactor: 调整构建配置和添加资源文件复制逻辑
1. feat(ui): 添加音乐托盘图标功能
1. fix: OllamaClient角色选择和优化消息处理
1. docs: 更新了一些文档
1. feat(setting): 添加日志等级设置功能
1. fix(KeyLabel): 修复窗口顶置失效问题
1. feat: 添加系统托盘双击事件处理
1. feat:自定义说话间隔 & 记录位置
1. fix(launcher.hpp): 优化对exe文件的启动逻辑

> [!CAUTION]
> 不兼容的文件：`configData.dat` 基本配置文件
> 检查更新不可用，因为相关API已更新！

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver20251118.9...20260107.10.16>

## ver20251118.9

### 更新内容

- fix: window no icon
- fix(core): 修改时间语录文件路径并添加新文件
- refactor: 删除不再使用的文本文件并更新语言设置
- refactor: 移除未使用函数并添加焦点检查功能
- style(core): 添加国际化支持并调整代码格式
- style: 修改文件头注释
- feat(translations): 修改配置文件
- fix(core): 修正窗口重置位置逻辑
- fix: 修改默认文本为 "Hello World!"
- fix(log): 日志信息国际化
- fix: 优化日志信息并添加多语言支持待办
- feat（todoNotification）: todo消息使用托盘提醒（可选）
- style/docs (thirdParty): 移除图标链接的图片标签，优化markdown显示
- docs: 修改README中的NOTE格式
- docs: 修改README中的术语和格式
- style(ui): 移除重复包含的data.hpp
- feat(ui): 实现菜单自动隐藏功能
- feat(setting): 添加静默启动选项

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver20251114-8...ver20251118.9>

## ver20251114-8

### 更新内容

- release: ver20251114-8 close <https://gitee.com/Pfolg/Pelr/issues/ID6AXW> [✔ empty push]
- release: ver20251114-8 close <https://gitee.com/Pfolg/Pelr/issues> [X empty push]
- chore: 删除旧部署脚本并更新版本号及忽略规则
- refactor(启动项目菜单): 使用CustomMenu替换QMenu
- docs: 添加README_en.txt和README_sourceforge.txt文档
- docs: update release log

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver20251031-7...ver20251114-8>

## ver20251031-7

### 更新内容

- feat: 新增调试和发布部署脚本
- style: assets 目录及其子目录的 Linguist 忽略设置
- docs: 添加仓库中不提供文件的说明
- fix: 更新版本号格式
- docs: 添加必要环境依赖说明及结构文件
- chore: ignore hand Third-party Libraries 3
- chore: ignore hand Third-party Libraries
- chore: ignore hand Third-party Libraries
- style: 删除了重复的注释行
- style: 添加第三方库忽略规则并移除旧路径
- docs: 移除Live2DCubismCore头文件引用
- feat(ui): 添加托盘报时功能
- fix: 删除不必要的assets复制步骤
- fix: 更改许可证标识为 GPLv3
- docs: 更新许可证链接格式
- fix: 更新项目许可证信息和版权声明

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver0.5-251018...ver20251031-7>

## ver0.5-251018

### 更新内容

- feat(ui): 添加启动项编辑标签与部署脚本
- fix(core/keyLabel): 移除静默模式keyLabel功能控制
- docs: 更新完整更新日志并调整版本号
- docs: 更新README.md中的安装说明
- docs: 添加完整版本更新日志
- feat(ui): 添加许可证检查功能

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver0.0.5...ver0.5-251018>

## ver0.0.5

**本次更新**：

- 完成项目适配Gitee
- ui优化
- 项目结构优化

ver0.0.5 已在 Windows 10 上**经测试**，可正常运行

本次更新为**非重大更新**，无新功能的添加或移除

### 更新内容

- refactor: 移除不必要的样式和代码调整
- style: 修正README.md的格式和注释内容
- refactor(ui): 将QMenu替换为CustomMenu并应用自定义样式
- feat(tray): 将启动菜单添加到托盘
- fix(ui): KeyLabel.cpp 单字符键名添加间距
- refactor: 移除冗余代码并优化slider访问方式
- refactor(ui): 窗口标志设置调整与.idea忽略规则更新
- docs: 更新文档中图片路径和编译配置说明
- feat(version): 添加对Gitee版本检查的支持
- docs: 更新构建流程中的默认设置
- feat(ui): 添加设置思考文本的功能
- refactor: 重构 launchByPathAsync 函数
- docs: 添加不必要的 Gitee 星标和 Fork 徽章
- docs(README): change links
- fix(README.md): 更新下载安装链接以及目录结构
- docs: 更新make_package.md中的配置设置
- docs: update
- docs: 更新第三方库信息和相关资源链接
- docs: 添加打包指南并更新相关配置
- docs: 允许创建空白问题
- docs: 更新问题模板中的文档链接和版本选项
- docs: 添加 Bug 反馈和功能建议模板
- docs: issue template
- add .gitee/ISSUE_TEMPLATE/config.yaml.
- chore: 删除不必要添加的Qt动态链接库文件
- chore: add necessary parts
- docs: 更新社区准则和安全报告链接

**完整的更新日志**: <https://gitee.com/Pfolg/Pelr/compare/ver0.0.4b...ver0.0.5>

## ver0.0.4b

由于GitHub账户被标记，项目迁移至Gitee，原仓库&账户可能无法访问。

本次更新：

- 添加媒体播放功能
- 原有的**关于**功能失效（将在后续更新修复）

你可以尝试访问：[GitHub上的提交](https://github.com/Pfolg/Plauncher/commits/main/)（可能无效） 以获取本版本的特性

## ver0.0.3

本次更新：

- 表情系统 <https://github.com/Pfolg/Plauncher/commit/700c7c3d0334a98db348f398dc7776a7b4bfe212>
- 检查更新的功能 <https://github.com/Pfolg/Plauncher/commit/67c397e2adae10e9c129780edf5cf07a143429a9>
- 局部优化

**Full Changelog**: <https://github.com/Pfolg/Plauncher/compare/ver0.0.2...ver0.0.3>

## ver0.0.2

> [!WARNING]
> 已知问题：有的额外窗口的大小不会根据设定自动调整，建议使用默认大小

- TODO功能实现
- UI优化
- 托盘功能优化
- 第二聊天功能

**基本使用方法**: <https://github.com/Pfolg/Plauncher/wiki/%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95>

**Full Changelog**: <https://github.com/Pfolg/Plauncher/compare/ver0.0.1...ver0.0.2>

## ver0.0.1

> [!WARNING]
> 已知问题：该版本的“随系统启动”功能无效，对基本使用无影响。

如果提示缺失\*.dll文件，可以下载选择性[bin_dlls](https://github.com/Pfolg/Plauncher/tree/main/bin_dlls)
内的对应缺失文件到程序根目录，并提交相关错误信息（界面->设置->关于->问题反馈）。

第一次运行提示缺失模型属于正常现象，在设置内完成初次配置后就不会提示了。
