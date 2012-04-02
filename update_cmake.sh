#!/bin/bash

FILES=`find ./src/ugdk/ -name *.cc -not -path "*/script/languages/*"`
LUA_FILES=`find ./src/ugdk/script/languages/lua -name *.cc`
PYTHON_FILES=`find ./src/ugdk/script/languages/python -name *.cc`
cd src/module
UGDK_MODULES=`find | grep -o "[^/]*ugdk_.*\.\(i\)$"`
PYRAMIDWORKS_MODULES=`find | grep -o "[^/]*pyramidworks_.*\.\(i\)$"`
cd ../..

echo "SET(UGDK_SRC " > ./src/src_list.cmake
for f in $FILES; do
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

echo "SET(UGDK_MODULE_SRC " > ./src/ugdk_module_list.cmake
for f in $UGDK_MODULES; do
	echo "    $f " >> ./src/ugdk_module_list.cmake
done
echo ")" >> ./src/ugdk_module_list.cmake
echo "Arquivo src/ugdk_module_list.txt criado com sucesso."

echo "SET(PYRAMIDWORKS_MODULE_SRC " > ./src/pyramidworks_module_list.cmake
for f in $PYRAMIDWORKS_MODULES; do
	echo "    $f " >> ./src/pyramidworks_module_list.cmake
done
echo ")" >> ./src/pyramidworks_module_list.cmake
echo "Arquivo src/pyramidworks_module_list.txt criado com sucesso."

