@echo off
call "%VS100COMNTOOLS%vsvars32.bat"

if not exist ugdk.vcxproj (cmake -DLINK_TYPE=STATIC .)

msbuild ugdk.vcxproj /p:Configuration=Debug
msbuild ugdk.vcxproj /p:Configuration=Release

pause