@REM this script is used to test the deployment of the project in debug mode
@REM only used for testing purposes
@REM some commands are outdated and may not work as expected or work badly

@REM echo off "run the bat file in the root directory of the project"

cd ..\
copy lib\Debug\*.dll cmake-build-debug\*.dll
xcopy FrameworkShaders cmake-build-debug\FrameworkShaders /E/H/C/I
xcopy Resources cmake-build-debug\Resources /E/H/C/I
xcopy SampleShaders cmake-build-debug\SampleShaders /E/H/C/I
xcopy assets cmake-build-debug\assets /E/H/C/I

cd  cmake-build-debug

"D:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe" Pelr.exe