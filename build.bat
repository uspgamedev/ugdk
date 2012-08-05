@echo off

if not exist ugdk.vcxproj (cmake -DLINK_TYPE=STATIC .)

cmake --build . --target libugdk --config Debug
cmake --build . --target libugdk --config Release

pause