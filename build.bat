call "%VS100COMNTOOLS%vsvars32.bat"
cmake -DLINK_TYPE=STATIC -DBUILD_TYPE=DEBUG .
msbuild ugdk.sln /p:Configuration=Debug
cmake -DLINK_TYPE=STATIC -DBUILD_TYPE=RELEASE .
msbuild ugdk.sln /p:Configuration=Release
pause