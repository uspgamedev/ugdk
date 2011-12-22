#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: $0 arch"
	exit 1
fi

directories=`find ../src/ -type d | sed 's/..\/src\///'`
headers=`find ../src/ -name "*.h" -or -name "*.th" -or -name "*.tcc" | sed 's/..\/src\///'`


for dir in $directories
do
    mkdir -p $1/usr/local/include/$dir
done

for header in $headers
do
    cp ../src/$header $1/usr/local/include/$header
done

mkdir -p $1/usr/local/lib/
cp ../lib/libugdk*.a ../lib/libugdk*.so $1/usr/local/lib/

