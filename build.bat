@echo off

if not exist libugdk.vcxproj (cmake .)

cmake --build . --target libugdk --config Debug
cmake --build . --target libugdk --config Release

pause