@echo off
:: mkdir deploy > nul
:: IF EXIST deploy rmdir /s /q deploy > nul
:: mkdir deploy\lib > nul
:: mkdir deploy\include > nul

echo Copying static library files...

xcopy lib\Release\ugdk.lib deploy\lib\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.lib deploy\lib\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.pdb deploy\lib\ /q /k /y > nul

echo Copying generated py files...

xcopy src\generated\*.py deploy\data\scripts\ugdk\ /q /k /y > nul

echo Copying header files...

xcopy "src\ugdk\*.h" "deploy\include\ugdk\" /e /c /q /k /y > nul
xcopy "src\ugdk\*.th" "deploy\include\ugdk\" /e /c /q /k /y > nul
xcopy "src\ugdk\*.tcc" "deploy\include\ugdk\" /e /c /q /k /y > nul

echo Copying generated Python scripts...

xcopy src\generated\*.py deploy\data\scripts\ugdk\ /q /k /y > nul

pause