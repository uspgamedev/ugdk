@echo off
mkdir deploy > nul
del /F /S /Q deploy > nul
mkdir deploy\lib > nul
mkdir deploy\include > nul

echo Copying static library files...

copy lib\Release\ugdk.lib deploy\lib\ugdk.lib > nul
copy lib\Debug\ugdk_d.lib deploy\lib\ugdk_d.lib > nul

echo Copying header files...

xcopy "src\ugdk\*.h" "deploy\include\ugdk\" /s /e /c > nul
xcopy "src\ugdk\*.th" "deploy\include\ugdk\" /s /e /c > nul
xcopy "src\ugdk\*.tcc" "deploy\include\ugdk\" /s /e /c > nul

pause