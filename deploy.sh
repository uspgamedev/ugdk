#!/bin/bash

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

cp lib/*.a lib/*.so $1/lib


