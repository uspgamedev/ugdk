call "build.bat"

echo Copying generated Asteroids VS project...

xcopy asteroids.vcxproj examples\asteroids-script\ /q /k /y > nul
xcopy asteroids.vcxproj.filters examples\asteroids-script\ /q /k /y > nul

echo Copying static library files...

xcopy lib\Release\ugdk.lib examples\asteroids-script\lib\Release\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.lib examples\asteroids-script\lib\Debug\ /q /k /y > nul
xcopy lib\Debug\ugdk_d.pdb examples\asteroids-script\lib\Debug\ /q /k /y > nul

echo Updating Python script modules.
xcopy src\generated\*.py examples\asteroids-script\data\scripts\ugdk\ /q /k /y > nul

pause