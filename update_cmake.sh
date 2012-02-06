#!/bin/bash

FILES=`find ./src/ugdk/ | grep "^.*\.\(h\|cc\)$"`
cd src/module
MODULES=`find | grep -o "[^/]*\.\(i\)$"`
cd ../..

echo "SET(FRAMEWORK_SRC " > ./src/src_list.cmake
for f in $FILES; do
	echo "    $f " >> ./src/src_list.cmake
done
echo ")" >> ./src/src_list.cmake
echo "Arquivo src/src_list.txt criado com sucesso."

echo "SET(MODULE_SRC " > ./src/module_list.cmake
for f in $MODULES; do
	echo "    $f " >> ./src/module_list.cmake
done
echo ")" >> ./src/module_list.cmake
echo "Arquivo src/module_list.txt criado com sucesso."

