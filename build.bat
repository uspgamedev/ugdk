@echo off

if not exist ugdk.vcxproj (cmake -DLINK_TYPE=STATIC .)

cmake --build . --target ugdk --config Debug
cmake --build . --target ugdk --config Release

pause