#!/bin/bash

function find_files() {
    echo "SET(GENERATED_SRC"
    find modules/$1/src -type f | sort
    echo ")"
    echo "SET(GENERATED_HEADERS"
    find modules/$1/include -type f | sort
    echo ")"
}

# CORE SOURCE
find_files ugdk-core > modules/ugdk-core/generated_src_list.cmake

# 2D SOURCE
find_files ugdk-2d > modules/ugdk-2d/generated_src_list.cmake