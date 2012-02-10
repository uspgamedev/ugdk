#!/bin/bash

help(){
    echo "Placeholder Help Message - Ask Wil"
}

if [ $# -lt 1 ] ; then
    echo "Error: Missing argument!"
    help
    exit 1;
fi

if [ $1 == "-h" -o $1 == "--help" ]; then
    help
    exit 0
fi


cd src

directories=`find . -type d`
headers=`find . -name "*.h" -or -name "*.th" -or -name "*.tcc"`

cd ..

for dir in $directories
do
    mkdir -p $1/include/$dir
done

for header in $headers
do
    cp src/$header $1/include/$header
done

mkdir -p $1/lib

cp -v lib/*.a lib/*.so lib/*.dylib $1/lib


