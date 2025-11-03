# PLauncher - Live2D Virtual Desktop Assistant
# https://gitee.com/Pfolg/plauncher
# https://sourceforge.net/projects/pfolg-plauncher/
# Copyright (c) 2025 SY Cheng
#
# GPL v3 License
# https://gnu.ac.cn/licenses/gpl-3.0.html


"""
cd ..\


copy lib\Debug\*.dll cmake-build-release\*.dll

xcopy FrameworkShaders cmake-build-release\FrameworkShaders /E/H/C/I
xcopy Resources cmake-build-release\Resources /E/H/C/I
xcopy SampleShaders cmake-build-release\SampleShaders /E/H/C/I
xcopy assets cmake-build-release\assets /E/H/C/I

cd  cmake-build-release

"D:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe" PLauncher.exe
"""
import os
import subprocess
import sys


def run_command(command, description=""):
    """执行命令并处理错误"""
    if description:
        print(f"\n=== {description} ===")

    print(f"执行命令: {command}")

    try:
        result = subprocess.run(command, shell=True, check=True,
                                capture_output=True, text=True)
        print("✓ 命令执行成功")
        if result.stdout:
            print(f"输出: {result.stdout}")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ 命令执行失败: {e}")
        if e.stderr:
            print(f"错误输出: {e.stderr}")
        return False


def copy_files(src_pattern, dst, description=""):
    """复制文件"""
    if description:
        print(f"\n=== {description} ===")

    print(f"复制: {src_pattern} -> {dst}")

    try:
        # 使用copy命令
        command = f'copy "{src_pattern}" "{dst}"'
        result = subprocess.run(command, shell=True, check=True,
                                capture_output=True, text=True)
        print("✓ 复制成功")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ 复制失败: {e}")
        return False


def copy_directory(src, dst, description=""):
    """复制目录"""
    if description:
        print(f"\n=== {description} ===")

    print(f"复制目录: {src} -> {dst}")

    try:
        # 使用xcopy命令，与原命令参数一致
        command = f'xcopy "{src}" "{dst}" /E/H/C/I'
        result = subprocess.run(command, shell=True, check=True,
                                capture_output=True, text=True)
        print("✓ 目录复制成功")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ 目录复制失败: {e}")
        return False


def main():
    print("开始执行部署脚本...")

    # 1. 切换到上级目录
    print(f"\n=== 切换到上级目录 ===")
    original_dir = os.getcwd()
    parent_dir = os.path.dirname(original_dir)
    os.chdir(parent_dir)
    print(f"当前工作目录: {os.getcwd()}")

    # 2. 复制DLL文件
    if not copy_files(r"lib\Debug\*.dll", r"cmake-build-release", "复制DLL文件"):
        print("警告: DLL文件复制可能有问题")

    # 3. 复制各个目录
    directories = [
        ("FrameworkShaders", "cmake-build-release\\FrameworkShaders"),
        ("Resources", "cmake-build-release\\Resources"),
        ("SampleShaders", "cmake-build-release\\SampleShaders"),
    ]

    all_success = True
    for src, dst in directories:
        if not copy_directory(src, dst, f"复制 {src} 目录"):
            all_success = False

    # 4. 切换到cmake-build-release目录
    print(f"\n=== 切换到cmake-build-release目录 ===")
    build_release_dir = os.path.join(os.getcwd(), "cmake-build-release")
    if os.path.exists(build_release_dir):
        os.chdir(build_release_dir)
        print(f"当前工作目录: {os.getcwd()}")
    else:
        print(f"✗ 错误: 目录不存在 - {build_release_dir}")
        all_success = False

    # 5. 运行windeployqt
    if all_success:
        windeployqt_path = r"D:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe"
        if not run_command(f'"{windeployqt_path}" PLauncher.exe', "运行windeployqt"):
            all_success = False

    # 输出最终结果
    if all_success:
        print("\n" + "=" * 50)
        print("✓ 所有操作完成！部署成功！")
        print("=" * 50)
    else:
        print("\n" + "=" * 50)
        print("⚠ 部署完成，但部分操作可能有问题")
        print("=" * 50)

    return all_success


if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
