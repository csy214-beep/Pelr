## ver0.0.5

**本次更新**：

+ 完成项目适配Gitee
+ ui优化
+ 项目结构优化

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

**完整的更新日志**: https://gitee.com/Pfolg/plauncher/compare/ver0.0.4b...ver0.0.5


## ver0.0.4b

由于GitHub账户被标记，项目迁移至Gitee，原仓库&账户可能无法访问。

本次更新：

- 添加媒体播放功能
- 原有的**关于**功能失效（将在后续更新修复）

你可以尝试访问：[GitHub上的提交](https://github.com/Pfolg/PLauncher/commits/main/)（可能无效） 以获取本版本的特性

## ver0.0.3

本次更新：

+ 表情系统 https://github.com/Pfolg/PLauncher/commit/700c7c3d0334a98db348f398dc7776a7b4bfe212
+ 检查更新的功能 https://github.com/Pfolg/PLauncher/commit/67c397e2adae10e9c129780edf5cf07a143429a9
+ 局部优化

**Full Changelog**: https://github.com/Pfolg/PLauncher/compare/ver0.0.2...ver0.0.3

## ver0.0.2

> [!WARNING]
> 已知问题：有的额外窗口的大小不会根据设定自动调整，建议使用默认大小

+ TODO功能实现
+ UI优化
+ 托盘功能优化
+ 第二聊天功能

**基本使用方法**: https://github.com/Pfolg/PLauncher/wiki/%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95

**Full Changelog**: https://github.com/Pfolg/PLauncher/compare/ver0.0.1...ver0.0.2

## ver0.0.1

> [!WARNING]
> 已知问题：该版本的“随系统启动”功能无效，对基本使用无影响。

如果提示缺失*.dll文件，可以下载选择性[bin_dlls](https://github.com/Pfolg/PLauncher/tree/main/bin_dlls)内的对应缺失文件到程序根目录，并提交相关错误信息（界面->设置->关于->问题反馈）。

第一次运行提示缺失模型属于正常现象，在设置内完成初次配置后就不会提示了。
