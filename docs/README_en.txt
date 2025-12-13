# PLauncher - Live2D Virtual Desktop Partner

repo site: https://gitee.com/Pfolg/plauncher
release log: https://gitee.com/Pfolg/plauncher/blob/master/docs/FullReleaseLog.md

PLauncher is an intelligent desktop virtual assistant based on Live2D technology, integrating AI dialogue, speech synthesis, quick launch, and personalized desktop companion features, providing you with an immersive desktop experience.

> NOTE
>
> This project is still in the development phase, and its functionality and stability may not be fully optimized. Please use with caution.
>
> This is a C++ project; Python is only used for the TTS server-side component.
>
> This is a non-profit, open-source project, developed out of personal interest by the author. It is free for anyone to use.

## Main Features

- Live2D Virtual Character - Supports Live2D models (only model3.json format), providing a vivid desktop companion experience
- Intelligent Dialogue - Integrates Ollama AI, supporting natural language interaction
- Expressions and Actions - Supports the model's built-in expressions and actions (if supported by the model), providing rich expression switching
- Speech Synthesis - Built-in Xunfei TTS service, providing high-quality voice feedback
- Launch Management - Visually manage launch applications, inherited from QuickTray
- Keyboard Monitoring - Displays key status, inherited from KeyMonitor
- Weather Service - OpenWeather integration, retrieves real-time weather information
- Highly Customizable - Rich settings options to meet personalized needs

More features under development...

Unsupported features (may not be supported in the future either):

- Lip sync
- Operating system operations
- Hotkeys
- Hot reloading of user configuration

## System Requirements

> NOTE
>
> For reference only

- Operating System: Windows 10/11 (Windows platform only)
- Processor: Dual-core processor or higher
- Memory: 4GB RAM or more
- Storage: At least 500MB of free space
- Graphics Card: Supports OpenGL 3.0 and above
- Python: 3.11 (Optional, only for TTS server)

## Quick Start

### Download and Install

1. Go to the Release page to download the latest version
2. Extract the zip file to any directory
3. Download `tts_server.exe` and place it in the program's root directory
4. Run `PLauncher.exe` to start the application

Additionally, due to Gitee's release limitations, you can visit sourceforge to view or download historical versions.

### First Run Configuration

1. Set Live2D Model Path (Required)
    - Configure the model path in Settings -> Basic Settings
    - Supports Live2D models in model3.json format
    - Model Downloads: Booth | Mod Zhi Wu

2. Configure TTS Service (Optional)
    - Apply for a Xunfei Open Platform account
    - Fill in API credentials in Settings -> TTS Configuration
    - Click 'Start Xunfei TTS Server' or manually run `tts_server.exe` in the directory

3. Set up AI Service (Optional)
    - Install Ollama
    - Select the model and role in Settings -> Ollama Integration

## Project Structure

```
PLauncher/
+-- CMakeLists.txt          # C++ project build configuration
+-- scripts/
�   +-- AUCF                # Deprecated module Archived Unused Cpp Files
�   +-- requirements.txt    # Python dependencies list
�   +-- tts_server.py       # TTS server
+-- src/                    # C++ source code
+-- Resources/              # Model resource files
+-- lib/                    # Third-party libraries
+-- LAppLive2D              # Live2D model loading library
+-- assets/                 # Resource files
+-- repo_assets/            # Repository-related resources
+-- SampleShaders/          # Example shaders
+-- FrameworkShaders/       # Framework shaders
+-- thirdParty/             # Third-party libraries
+-- docs/                   # Documentation files
+-- LICENSE.md              # License file
+-- README.md               # Project description file
+-- SUPPORT.md              # Contribution guide
+-- SECURITY.md             # Security notice file
+-- build/                  # Build output directory
```

> NOTE
>
> For files not provided in the repository, please see necessaryParyStructure

## Technology Stack

### C++ Core Components

- Qt 5.15.2 - Cross-platform application framework
- OpenGL - Graphics rendering (GLEW + GLFW)
- Live2D Cubism - 2D animation rendering engine (only supports model3.json format)
- STB Library - Image processing functionality

### Python Toolchain

- Python 3.11 - Development environment
- PyInstaller - Application packaging and distribution
- Pillow - Image processing
- pystray - System tray integration
- websocket-client - Network communication

## Development Build

### Environment Preparation

1. Install Qt 5.15.2 (MingW81_64 version)
2. Install Python 3.11 and required dependencies:
   pip install -r scripts/requirements.txt
3. Configure C++ compilation environment (CMake + MingW)

### Compilation Steps

> Refer to the Build Process documentation

**Developed using CLion, the effectiveness of the following commands is not guaranteed**

git clone https://gitee.com/Pfolg/plauncher.git
cd PLauncher

mkdir build && cd build

cmake -G "MinGW Makefiles" ..

mingw32-make

## User Guide

> CAUTION
>
> Please do not upload any content from the `user` folder

> NOTE
>
> For detailed functional instructions, please refer to the Wiki

### Basic Operations

- Main Interface Navigation: Use the left sidebar to switch functional modules
- Chat Function: Enter messages in the chat interface to interact with the virtual character (Ollama AI and TTS functions are independent of each other)
- Launch Item Management: Manage custom launch programs

## Contributing

We welcome contributions in various forms!

- Report Bugs
- Suggest New Features
- Write Code
- Provide Feedback
- Issue Feedback
- Help Center

## License

> NOTE
>
> This project is released under the GNU General Public License v3.0.
>
> See the LICENSE file for details.
>
> Distribution of past versions (Release) still follows the original license (MIT), but their source code (the parts written by this project's developers) uses the GPLv3 license.

Note: Some components use different licenses:

- Live2D Cubism SDK uses a proprietary license
- Qt framework uses LGPL/GPL license
- For other third-party libraries, see the Third-Party Library List

## Acknowledgments

https://gitee.com/Pfolg/plauncher/wikis/IMPORTANT

Thanks to the following projects and communities for their support:

- Live2D Cubism - Provides excellent 2D animation technology
- Qt Framework - Powerful cross-platform development framework
- Ollama - Local AI model deployment
- Xunfei Open Platform - High-quality speech synthesis service
- Support from all contributors and users

## Technical Support

- Issue Feedback
- Wiki Documentation
- SUPPORT
- Security Policy