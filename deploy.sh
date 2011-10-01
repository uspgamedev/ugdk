#!/bin/bash

cd src

directories=`find . -type d`
headers_simple=`find . -name *.h`
headers_template=`find . -name *.th`
headers_template2=`find . -name *.tcc`
headers="$headers_simple $headers_template $headers_template2"

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


