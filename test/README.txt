For Windows:
- Place the ugdk.lib, gtest.lib and gtest_main.lib files inside the lib folder.
- Open ugdk_tests.sln and set build type for Release
- Compile and run!

ugdk.lib:
1. Generate the VS2010 project files for the ugdk with BUILD_TYPE Release and LINK_TYPE Static (defaults).
2. Compile using build type Release.
3. The file you want will be located in $(SOLUTIONDIR)\lib\Release\ugdk.lib

gtest.lib and gtest_main.lib:
1. Download gtest-1.6.0.zip from http://code.google.com/p/googletest/downloads/list
2. Unzip and open msvc\gtest.sln with VS2010. Convert the file with default configurations.
3. Set build type to Release and compile.
4. The files you want will be located in gtest-1.6.0\msvc\gtest\Release.