@REM in the root directory of the project, run "call scripts\deploy-r.bat"

echo off "run the bat file in the root directory of the project"
@REM cd ..\
copy lib\Debug\*.dll cmake-build-release\*.dll
xcopy FrameworkShaders cmake-build-release\FrameworkShaders /E/H/C/I
xcopy Resources cmake-build-release\Resources /E/H/C/I
xcopy SampleShaders cmake-build-release\SampleShaders /E/H/C/I
xcopy assets cmake-build-release\assets /E/H/C/I

cd cmake-build-release

"D:\Qt\5.15.2\mingw81_64\bin\windeployqt.exe" Pelr.exe

pause