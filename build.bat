@echo off
call "%VS100COMNTOOLS%vsvars32.bat"

echo Running CMake for DEBUG
cmake -DLINK_TYPE=STATIC -DBUILD_TYPE=DEBUG .
msbuild ugdk_d.vcxproj /p:Configuration=Debug

echo Running CMake for RELEASE
cmake -DLINK_TYPE=STATIC -DBUILD_TYPE=RELEASE .
msbuild ugdk.vcxproj /p:Configuration=Release

pause