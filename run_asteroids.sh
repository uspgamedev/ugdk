#!/bin/bash

# Compile tests.
make -j3 asteroids

# Update Python script modules.
cp -v src/generated/*.py examples/asteroids-script/data/scripts/ugdk/

# Run tests.
cd examples/asteroids-script/
./asteroids
cd ../..

