call "%VS100COMNTOOLS%vsvars32.bat"
cmake -DBUILD_TYPE=DEBUG .
msbuild ugdk.sln /p:Configuration=Debug
cmake -DBUILD_TYPE=RELEASE .
msbuild ugdk.sln /p:Configuration=Release
pause