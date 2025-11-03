echo off "run the bat file in the root directory of the project"

@REM cd ..\
copy lib\Debug\*.dll cmake-build-release\*.dll
xcopy FrameworkShaders cmake-build-debug\FrameworkShaders /E/H/C/I
xcopy Resources cmake-build-debug\Resources /E/H/C/I
xcopy SampleShaders cmake-build-debug\SampleShaders /E/H/C/I

cd  cmake-build-debug

"D:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe" PLauncher.exe