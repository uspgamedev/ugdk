@echo off
:: mkdir deploy > nul
:: IF EXIST deploy rmdir /s /q deploy > nul
:: mkdir deploy\lib > nul
:: mkdir deploy\include > nul

echo Copying static library files...

xcopy lib\Release\ugdk.lib deploy\lib\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.lib deploy\lib\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.pdb deploy\lib\ /q /k /y > nul

echo Copying header files...

xcopy "src\*.h" "deploy\include\" /e /c /q /k /y > nul
xcopy "src\*.th" "deploy\include\" /e /c /q /k /y > nul
xcopy "src\*.tcc" "deploy\include\" /e /c /q /k /y > nul

echo Copying generated Python scripts...

xcopy src\generated\*.py deploy\data\scripts\ugdk\ /q /k /y > nul

pause