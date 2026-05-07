#!/usr/bin/env python3
"""
生成 NOTICE 文件的脚本。
从内置的第三方库信息中读取每个库的许可证文件，
并将内容直接嵌入到最终生成的纯文本 NOTICE 文件中。
"""

import os
from datetime import datetime

# ============================================================
# 项目自身信息
# ============================================================
PROJECT_NAME = "Pelr"
PROJECT_AUTHOR = "SY Cheng"  # 请修改为实际信息
PROJECT_HOMEPAGE = "https://github.com/csy214-beep/Pelr"

# ============================================================
# MIT 许可证全文 (用于项目自身声明)
# ============================================================
MIT_LICENSE_TEXT = """MIT License

Copyright (c) {year} {author}

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
""".format(year=datetime.now().year, author=PROJECT_AUTHOR)

# ============================================================
# 第三方库信息列表
# 每个条目必须包含 'name' 和 'license_path' (可为空字符串)
# 如果 'license_path' 不为空，脚本会尝试读取该文件内容
# 'version', 'copyright', 'homepage' 可为空字符串
# 'extra_notes' 用于补充说明（可选）
# ============================================================
THIRD_PARTY_LIBS = [
    {
        "name": "Qt",
        "version": "5.15.2",
        "license_type": "LGPLv3 / GPLv2",
        "copyright": "The Qt Company Ltd.",
        "homepage": "https://www.qt.io/",
        "license_path": "",  # Qt不在仓库中
        "extra_notes": (
            "Qt is licensed under LGPLv3. This software dynamically links to Qt libraries.\n"
            "You may obtain the source code of Qt and its full license texts from https://www.qt.io/."
        ),
    },
    {
        "name": "Live2D Cubism Native Framework",
        "version": "5-r.4.1",
        "license_type": "Live2D Open Software License",
        "copyright": "Live2D Inc.",
        "homepage": "https://www.live2d.com/",
        "license_path": "thirdParty/Framework/LICENSE.md",
        "extra_notes": (
            "If your annual revenue exceeds 10 million JPY, a separate Cubism SDK Release License is required.\n"
            "See https://www.live2d.com/en/download/cubism-sdk/release-license/"
        ),
    },
    {
        "name": "Live2D Cubism Core",
        "version": "Included with SDK",
        "license_type": "Live2D Proprietary Software License",
        "copyright": "Live2D Inc.",
        "homepage": "https://www.live2d.com/",
        "license_path": "",  # 专有许可，不应放入仓库
        "extra_notes": (
            "Cubism Core is under a proprietary license. It must not be redistributed in public repositories.\n"
            "Obtain it from the official Live2D SDK. License text:\n"
            "https://www.live2d.com/eula/live2d-proprietary-software-license-agreement_en.html"
        ),
    },
    {
        "name": "GLEW",
        "version": "2.1.0",
        "license_type": "Modified BSD License",
        "copyright": "Milan Ikits, Marcelo E. Magallon, Lev Povalahev",
        "homepage": "http://glew.sourceforge.net/",
        "license_path": "thirdParty/glew/LICENSE.txt",
        "extra_notes": "",
    },
    {
        "name": "GLFW",
        "version": "3.4.0",
        "license_type": "zlib/libpng License",
        "copyright": "Marcus Geelnard, Camilla Löwy",
        "homepage": "https://www.glfw.org/",
        "license_path": "thirdParty/glfw/LICENSE.md",
        "extra_notes": "",
    },
    {
        "name": "kissfft",
        "version": "Latest",
        "license_type": "BSD-3-Clause",
        "copyright": "Mark Borgerding",
        "homepage": "https://github.com/mborgerding/kissfft",
        "license_path": "thirdParty/kissfft/COPYING",
        "extra_notes": "",
    },
    {
        "name": "stb",
        "version": "Latest",
        "license_type": "MIT / Public Domain",
        "copyright": "Sean Barrett",
        "homepage": "https://github.com/nothings/stb",
        "license_path": "",  # stb许可证嵌入在头文件中，无独立文件
        "extra_notes": (
            "stb libraries are in the public domain, and also available under the MIT License.\n"
            "See the copyright notice at the top of each header file."
        ),
    },
    {
        "name": "VOICEVOX CORE",
        "version": "0.16.4",
        "license_type": "MIT (with additional terms)",
        "copyright": "Hiroshiba Kazuyuki",
        "homepage": "https://github.com/VOICEVOX/voicevox_core",
        "license_path": "thirdParty/voicevox_core/c_api/LICENSE",
        "extra_notes": (
            "YOU MUST INCLUDE THE CREDIT 'VOICEVOX: Hiroshiba Kazuyuki' when using the generated audio.\n"
            "Additional terms: thirdParty/voicevox_core/c_api/TERMS.txt"
        ),
    },
    {
        "name": "ONNX Runtime",
        "version": "Included with voicevox_core",
        "license_type": "MIT",
        "copyright": "Microsoft Corporation",
        "homepage": "https://onmicrosoft.com/",
        "license_path": "thirdParty/voicevox_core/onnxruntime/TERMS.txt",
        "extra_notes": "",
    },
]


# ============================================================
# 辅助函数
# ============================================================
def read_license_file(relative_path):
    """读取许可证文件内容，如果文件不存在则返回提示信息。"""
    if not relative_path:
        return None
    if not os.path.exists(relative_path):
        return f"[WARNING] License file not found at: {relative_path}\n"
    try:
        with open(relative_path, 'r', encoding='utf-8', errors='replace') as f:
            return f.read()
    except Exception as e:
        return f"[ERROR] Could not read license file {relative_path}: {e}\n"


def generate_notice():
    """生成纯文本 NOTICE 文件内容。"""
    lines = []
    # 标题
    lines.append("NOTICE - Third Party Dependencies and Licenses")
    lines.append("=" * 60)
    lines.append(f"Project: {PROJECT_NAME}")
    lines.append(f"Homepage: {PROJECT_HOMEPAGE}")
    lines.append("")

    # 第一部分：项目自身的 MIT 许可证
    lines.append("1. Project License (MIT)")
    lines.append("-" * 40)
    lines.append(MIT_LICENSE_TEXT)
    lines.append("")

    # 第二部分：第三方库
    lines.append("2. Third Party Libraries")
    lines.append("-" * 40)

    for idx, lib in enumerate(THIRD_PARTY_LIBS, 1):
        name = lib["name"]
        version = lib["version"]
        license_type = lib["license_type"]
        copyright_holder = lib["copyright"]
        homepage = lib["homepage"]
        extra = lib.get("extra_notes", "")
        license_path = lib.get("license_path", "")

        lines.append(f"--- Library {idx}: {name} ---")
        if version:
            lines.append(f"Version: {version}")
        lines.append(f"License Type: {license_type}")
        lines.append(f"Copyright (c) {copyright_holder}")
        lines.append(f"Website: {homepage}")
        lines.append("")

        # 读取许可证文件内容
        license_text = read_license_file(license_path)
        if license_text:
            lines.append("---- Included License Text ----")
            lines.append(license_text.strip())
            lines.append("")
        else:
            lines.append("")
            # 对于无法直接读取的许可，提供 extra_notes
            if extra:
                lines.append("Additional Notes:")
                lines.append(extra.strip())
                lines.append("")
        if extra and license_text:
            # 如果同时存在许可证文本和额外说明，则追加额外说明
            lines.append("Additional Notes:")
            lines.append(extra.strip())
            lines.append("")

    # 尾部说明
    lines.append("3. Additional Information")
    lines.append("-" * 40)
    lines.append(
        "Full license texts for the above libraries are maintained in their respective"
    )
    lines.append("subdirectories under `thirdParty/` in the source repository.")
    lines.append(
        "This NOTICE file is auto-generated by `generate_notice.py`. Please update the"
    )
    lines.append("script when dependencies change.")
    lines.append("")

    return "\n".join(lines)


# ============================================================
# 主程序入口
# ============================================================
if __name__ == "__main__":
    output_path = "NOTICE"
    content = generate_notice()
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"NOTICE file generated successfully: {output_path}")
