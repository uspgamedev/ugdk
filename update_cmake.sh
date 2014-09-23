#!/bin/bash

FILES=`find src/ugdk src/pyramidworks \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) -not \( -path "*/script/languages/*" -o -path "*3D*" -o -path "*/graphic/*" -o -path "*/text/*" -o -path "*/ui/*" \) | grep -v config.h | sort`
FILES_2D=`find src/ugdk/{graphic,text,ui} \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) | sort`
FILES_BULLETWORKS=`find src/bulletworks \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) | sort`
FILES_3D=`find src/ugdk \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) -path "*3D*" | sort`
LUA_FILES=`find src/ugdk/script/languages/lua -name "*.cc" -o -name "*.h" | sort`
PYTHON_FILES=`find src/ugdk/script/languages/python -name "*.cc" -o -name "*.h" | sort`
MODULES=`find src/module -name "ugdk_*.i" -o -name "pyramidworks_*.i" | sort -h`

echo "SET(UGDK_SRC " > ./src/src_list.cmake
for f in $FILES; do
	echo "    $f " >> ./src/src_list.cmake
done
echo ")" >> ./src/src_list.cmake
echo "SET(UGDK_2D_SRC " >> ./src/src_list.cmake
for f in $FILES_2D; do
	echo "    $f " >> ./src/src_list.cmake
done
echo ")" >> ./src/src_list.cmake
echo "SET(UGDK_3D_SRC " >> ./src/src_list.cmake
for f in $FILES_3D $FILES_BULLETWORKS; do
	echo "    $f " >> ./src/src_list.cmake
done
echo ")" >> ./src/src_list.cmake
echo "Arquivo src/src_list.txt criado com sucesso."

echo "SET(UGDK_LUA_SRC " > ./src/lua_src_list.cmake
for f in $LUA_FILES; do
  echo "    $f " >> ./src/lua_src_list.cmake
done
echo ")" >> ./src/lua_src_list.cmake
echo "Arquivo src/lua_src_list.txt criado com sucesso."

echo "SET(UGDK_PYTHON_SRC " > ./src/py_src_list.cmake
for f in $PYTHON_FILES; do
	echo "    $f " >> ./src/py_src_list.cmake
done
echo ")" >> ./src/py_src_list.cmake
echo "Arquivo src/py_src_list.txt criado com sucesso."

echo "SET(UGDK_MODULE_SRC " > ./src/module_list.cmake
for f in $MODULES; do
	echo "    $f " >> ./src/module_list.cmake
done
echo ")" >> ./src/module_list.cmake
echo "Arquivo src/module_list.txt criado com sucesso."
