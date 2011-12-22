#!/bin/bash

if [ "$1" == "" ]; then
	echo "Usage: $0 arch"
	exit 1
fi

VER="0.2.0"
ARCH="$1"

directories=`find ../src/ -type d | sed 's/..\/src\///'`
headers=`find ../src/ -name "*.h" -or -name "*.th" -or -name "*.tcc" | sed 's/..\/src\///'`

cd ..

cmake -DBUILD_TYPE=DEBUG -DLINK_TYPE=STATIC .
make clean
make -j
cmake -DBUILD_TYPE=DEBUG -DLINK_TYPE=SHARED .
make -j
cmake -DBUILD_TYPE=RELEASE -DLINK_TYPE=STATIC .
make -j
cmake -DBUILD_TYPE=RELEASE -DLINK_TYPE=SHARED .
make -j

cd package

mkdir -p $ARCH/DEBIAN/
cp control $ARCH/DEBIAN/
echo "Architecture: $ARCH" >> $1/DEBIAN/control


for dir in $directories
do
    mkdir -p $ARCH/usr/local/include/$dir
done

for header in $headers
do
    cp ../src/$header $ARCH/usr/local/include/$header
done

mkdir -p $ARCH/usr/local/lib/
cp ../lib/libugdk*.a ../lib/libugdk*.so $ARCH/usr/local/lib/

dpkg-deb -b $ARCH "ugdk-$VER-$ARCH.deb"

