#!/bin/bash

# Compile tests.
make -j3 script_customtest

# Update Python script modules.
cp -v src/generated/*.py test/data/scripts/ugdk/

# Run tests.
cd test/
./script_customtest
cd ..

