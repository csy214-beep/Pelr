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

## ver20251118.9

### 更新内容

* fix: window no icon
* fix(core): 修改时间语录文件路径并添加新文件
* refactor: 删除不再使用的文本文件并更新语言设置
* refactor: 移除未使用函数并添加焦点检查功能
* style(core): 添加国际化支持并调整代码格式
* style: 修改文件头注释
* feat(translations): 修改配置文件
* fix(core): 修正窗口重置位置逻辑
* fix: 修改默认文本为 "Hello World!"
* fix(log): 日志信息国际化
* fix: 优化日志信息并添加多语言支持待办
* feat（todoNotification）: todo消息使用托盘提醒（可选）
* style/docs (thirdParty): 移除图标链接的图片标签，优化markdown显示
* docs: 修改README中的NOTE格式
* docs: 修改README中的术语和格式
* style(ui): 移除重复包含的data.hpp
* feat(ui): 实现菜单自动隐藏功能
* feat(setting): 添加静默启动选项

**完整的更新日志**: <https://gitee.com/Pfolg/plauncher/compare/ver20251114-8...ver20251118.9>

## ver20251114-8

### 更新内容

* release: ver20251114-8 close <https://gitee.com/Pfolg/plauncher/issues/ID6AXW> [✔ empty push]
* release: ver20251114-8 close <https://gitee.com/Pfolg/plauncher/issues> [X empty push]
* chore: 删除旧部署脚本并更新版本号及忽略规则
* refactor(启动项目菜单): 使用CustomMenu替换QMenu
* docs: 添加README_en.txt和README_sourceforge.txt文档
* docs: update release log

**完整的更新日志**: <https://gitee.com/Pfolg/plauncher/compare/ver20251031-7...ver20251114-8>

## ver20251031-7

### 更新内容

* feat: 新增调试和发布部署脚本
* style: assets 目录及其子目录的 Linguist 忽略设置
* docs: 添加仓库中不提供文件的说明
* fix: 更新版本号格式
* docs: 添加必要环境依赖说明及结构文件
* chore: ignore hand Third-party Libraries 3
* chore: ignore hand Third-party Libraries
* chore: ignore hand Third-party Libraries
* style: 删除了重复的注释行
* style: 添加第三方库忽略规则并移除旧路径
* docs: 移除Live2DCubismCore头文件引用
* feat(ui): 添加托盘报时功能
* fix: 删除不必要的assets复制步骤
* fix: 更改许可证标识为 GPLv3
* docs: 更新许可证链接格式
* fix: 更新项目许可证信息和版权声明

**完整的更新日志**: <https://gitee.com/Pfolg/plauncher/compare/ver0.5-251018...ver20251031-7>

## ver0.5-251018

### 更新内容

* feat(ui): 添加启动项编辑标签与部署脚本
* fix(core/keyLabel): 移除静默模式keyLabel功能控制
* docs: 更新完整更新日志并调整版本号
* docs: 更新README.md中的安装说明
* docs: 添加完整版本更新日志
* feat(ui): 添加许可证检查功能

**完整的更新日志**: <https://gitee.com/Pfolg/plauncher/compare/ver0.0.5...ver0.5-251018>

## ver0.0.5

**本次更新**：

* 完成项目适配Gitee
* ui优化
* 项目结构优化

ver0.0.5 已在 Windows 10 上**经测试**，可正常运行

本次更新为**非重大更新**，无新功能的添加或移除

### 更新内容

* refactor: 移除不必要的样式和代码调整
* style: 修正README.md的格式和注释内容
* refactor(ui): 将QMenu替换为CustomMenu并应用自定义样式
* feat(tray): 将启动菜单添加到托盘
* fix(ui): KeyLabel.cpp 单字符键名添加间距
* refactor: 移除冗余代码并优化slider访问方式
* refactor(ui): 窗口标志设置调整与.idea忽略规则更新
* docs: 更新文档中图片路径和编译配置说明
* feat(version): 添加对Gitee版本检查的支持
* docs: 更新构建流程中的默认设置
* feat(ui): 添加设置思考文本的功能
* refactor: 重构 launchByPathAsync 函数
* docs: 添加不必要的 Gitee 星标和 Fork 徽章
* docs(README): change links
* fix(README.md): 更新下载安装链接以及目录结构
* docs: 更新make_package.md中的配置设置
* docs: update
* docs: 更新第三方库信息和相关资源链接
* docs: 添加打包指南并更新相关配置
* docs: 允许创建空白问题
* docs: 更新问题模板中的文档链接和版本选项
* docs: 添加 Bug 反馈和功能建议模板
* docs: issue template
* add .gitee/ISSUE_TEMPLATE/config.yaml.
* chore: 删除不必要添加的Qt动态链接库文件
* chore: add necessary parts
* docs: 更新社区准则和安全报告链接

**完整的更新日志**: <https://gitee.com/Pfolg/plauncher/compare/ver0.0.4b...ver0.0.5>

## ver0.0.4b

由于GitHub账户被标记，项目迁移至Gitee，原仓库&账户可能无法访问。

本次更新：

* 添加媒体播放功能
* 原有的**关于**功能失效（将在后续更新修复）

你可以尝试访问：[GitHub上的提交](https://github.com/Pfolg/PLauncher/commits/main/)（可能无效） 以获取本版本的特性

## ver0.0.3

本次更新：

* 表情系统 <https://github.com/Pfolg/PLauncher/commit/700c7c3d0334a98db348f398dc7776a7b4bfe212>
* 检查更新的功能 <https://github.com/Pfolg/PLauncher/commit/67c397e2adae10e9c129780edf5cf07a143429a9>
* 局部优化

**Full Changelog**: <https://github.com/Pfolg/PLauncher/compare/ver0.0.2...ver0.0.3>

## ver0.0.2

> [!WARNING]
> 已知问题：有的额外窗口的大小不会根据设定自动调整，建议使用默认大小

* TODO功能实现
* UI优化
* 托盘功能优化
* 第二聊天功能

**基本使用方法**: <https://github.com/Pfolg/PLauncher/wiki/%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95>

**Full Changelog**: <https://github.com/Pfolg/PLauncher/compare/ver0.0.1...ver0.0.2>

## ver0.0.1

> [!WARNING]
> 已知问题：该版本的“随系统启动”功能无效，对基本使用无影响。

如果提示缺失*.dll文件，可以下载选择性[bin_dlls](https://github.com/Pfolg/PLauncher/tree/main/bin_dlls)
内的对应缺失文件到程序根目录，并提交相关错误信息（界面->设置->关于->问题反馈）。

第一次运行提示缺失模型属于正常现象，在设置内完成初次配置后就不会提示了。
