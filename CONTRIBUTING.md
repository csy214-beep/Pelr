# Contributing to PLauncher

感谢您有兴趣为 PLauncher 做贡献！

We are grateful for any contributions, whether it's code, documentation, design, or ideas.

我们非常感谢任何形式的贡献，无论是代码、文档、设计还是想法。

Please take a moment to review this document to make the contribution process smooth and effective for everyone.

请花一点时间阅读本文档，以便让贡献过程对每个人都更加顺畅和高效。

## Code of Conduct

We adopt the Contributor Covenant for our community. By participating, you are expected to uphold this code.

我们的社区遵循贡献者公约。参与即表示您同意遵守该准则。

## How Can I Contribute?

### Reporting Bugs

>**TIP**
>
>Before creating a bug report, please check the existing issues to avoid duplicates.
>
>在创建错误报告之前，请检查现有的 Issues 以避免重复。

Use the Bug Report template to file a new issue. Please include as many details as possible:

使用错误报告模板来提交新 issue。请尽可能包含详细信息：

*   A clear description of the bug

    清晰的错误描述
    
*   Steps to reproduce

    复现步骤

*   Expected vs. Actual behavior

    预期行为与实际行为
    
*   Screenshots (if applicable)

    截图（如果适用）
    
*   Your environment (OS, PLauncher version, etc.)

    您的环境（操作系统、PLauncher 版本等）
    
*   Relevant log output (check the log folder in the application directory)

    相关日志输出（请检查应用程序目录中的 log 文件夹）
    

### Suggesting Enhancements

>**NOTE**
>
>We love new ideas! Use the Feature Request template to suggest a new feature or enhancement.
>
>我们欢迎新想法！使用功能请求模板来建议新功能或改进。

A good enhancement suggestion includes:

一个好的功能建议包括：

*   A clear and detailed description of the proposed feature.

    对提议功能的清晰详细的描述。
    
*   The problem it solves or the use case it enables.

    它解决了什么问题或实现了什么用例。
    
*   Any alternative solutions you've considered.

    您考虑过的任何替代解决方案。
    
*   Mockups or examples, if it's a UI change.

    如果是 UI 更改，请提供模型或示例。

### Pull Requests

We welcome pull requests. Follow these steps:

我们欢迎拉取请求。请遵循以下步骤：

+ Fork the repository and create your branch from main.

    Fork 本仓库并从 main 分支创建您的特性分支。

+ Set up the development environment as described below.

   按照下面的说明设置开发环境。

+ Make your changes.

  进行您的更改。

+ Test your changes thoroughly.

  全面测试您的更改。

+ Commit your changes. Write clear and descriptive commit messages.

  提交您的更改。请编写清晰且具有描述性的提交信息。

+ Push to your fork and open a Pull Request. Fill out the PR template completely.

  推送到您的 Fork 仓库并发起一个拉取请求。请完整填写 PR 模板。

+ Discuss and review. We may suggest changes. Please be responsive to feedback.

  讨论和审查。我们可能会建议更改。请及时回应反馈。

## Development Environment Setup

### Prerequisites

*   Git
*   CMake (>= 3.16)
*   MinGW-w64 (GCC compiler suite)
*   Qt 5.15.2 (MingW81_64 version) - Ensure the path is set in CMAKE_PREFIX_PATH.
*   Python 3.11 (TTS Server)

### Build Steps

1.  Clone your fork

   克隆你的 Fork 仓库

3.  Configure with CMake

   使用 CMake 配置

5.  Build the project

   构建项目

7.  Install Python dependencies for scripts

   为脚本安装 Python 依赖

## General Guidelines

*   Follow the existing code style and patterns in the project.

    遵循项目中现有的代码风格和模式。
    
*   Use descriptive variable and function names.

    使用描述性的变量和函数名。
    
*   Comment complex sections of code.

    对复杂的代码段进行注释。
    
*   Ensure your code changes do not break existing functionality.

    确保您的代码更改不会破坏现有功能。
    
*   Test your changes on the target platform (Windows).

    在目标平台（Windows）上测试您的更改。

## License

>**NOTE**
>
>By contributing to PLauncher, you agree that your contributions will be licensed under the project's MIT License.
>
>通过向 PLauncher 贡献，您同意您的贡献将根据项目的 MIT 许可证进行许可。

## Need Help?

If you have any questions, feel free to:

如果您有任何问题，请随时：

*   Open a Discussion.

    发起一个讨论。
    
*   Check the Wiki for more detailed documentation.

    查看 Wiki 获取更详细的文档。

Thank you for contributing!

感谢您的贡献！
