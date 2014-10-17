#!/bin/bash

function find_files() {
    UPPER_NAME=$(<<< $1 tr '[:lower:]' '[:upper:]')
    echo "SET(${UPPER_NAME}_SRC"
    find modules/$1/src -type f \( -name "*.cc" \) | sort
    echo ")"
    echo "SET(${UPPER_NAME}_HEADERS"
    find modules/$1/include -type f \( -name "*.h" \) | sort
    echo ")"
}

# CORE SOURCE
find_files ugdk-core > modules/ugdk-core/src_list.cmake

# SCRIPT SOURCE
find_files ugdk-script | grep -v languages > modules/ugdk-script/src_list.cmake

# 2D SOURCE
find_files ugdk-2d > modules/ugdk-2d/src_list.cmake

# 3D SOURCE
find_files ugdk-3d > modules/ugdk-3d/src_list.cmake

# LUA SOURCE
echo "SET(UGDK_LUA_SRC " > modules/ugdk-script/lua_src_list.cmake
find modules/ugdk-script/src/languages/lua -type f  | sort >> modules/ugdk-script/lua_src_list.cmake
find modules/ugdk-script/include/ugdk/script/languages/lua -type f  | sort >> modules/ugdk-script/lua_src_list.cmake
echo ")" >> modules/ugdk-script/lua_src_list.cmake

# PYTHON SOURCE
echo "SET(UGDK_PYTHON_SRC " > modules/ugdk-script/python_src_list.cmake
find modules/ugdk-script/src/languages/python -type f  | sort >> modules/ugdk-script/python_src_list.cmake
find modules/ugdk-script/include/ugdk/script/languages/python -type f  | sort >> modules/ugdk-script/python_src_list.cmake
echo ")" >> modules/ugdk-script/python_src_list.cmake

###############
# MOUDULE LIST
echo "SET(UGDK_MODULE_SRC " > src/swig_module_list.cmake
find src/module \( -name "ugdk_*.i" -o -name "pyramidworks_*.i" \) -a -not \( -name "*2D*" -o -name "*3D*" \) | sort -h >> src/swig_module_list.cmake
echo ")" >> src/swig_module_list.cmake
echo "Arquivo src/swig_module_list.txt criado com sucesso."

# 2d
echo "SET(UGDK_MODULE_2D_SRC " >> src/swig_2d_module_list.cmake
find src/module -name "*2D*" -a \( -name "ugdk_*.i" -o -name "pyramidworks_*.i" \) | sort -h >> src/swig_2d_module_list.cmake
echo ")" >> src/swig_2d_module_list.cmake

# 3d
echo "SET(UGDK_MODULE_3D_SRC " >> src/swig_3d_module_list.cmake
find src/module -name "*3D*" -a \( -name "ugdk_*.i" -o -name "pyramidworks_*.i" \) | sort -h >> src/swig_3d_module_list.cmake
echo ")" >> src/swig_3d_module_list.cmake

