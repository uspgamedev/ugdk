#!/bin/bash

# CORE SOURCE
echo "SET(UGDK_SRC " > src/src_list.cmake
find src/ugdk src/pyramidworks \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) -not \( -path "*/script/languages/*" -o -path "*/3D/*" -o -path "*/2D/*" -o -path "*/graphic/*" -o -path "*/text/*" -o -path "*/ui/*" -o -path "*/internal/*" \) | grep -v config.h | sort >> src/src_list.cmake
echo ")" >> src/src_list.cmake
echo "Arquivo src/src_list.txt criado com sucesso."

# 2D SOURCE
echo "SET(UGDK_2D_SRC " > src/2d_module_src_list.cmake
find src/ugdk/{graphic,text,ui,internal} \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) | sort >> src/2d_module_src_list.cmake
find src/ugdk \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) -path "*/2D/*" | sort >> src/2d_module_src_list.cmake
echo ")" >> src/2d_module_src_list.cmake
echo "Arquivo src/2d_module_src_list.txt criado com sucesso."

# 3D SOURCE
echo "SET(UGDK_3D_SRC " > src/3d_module_src_list.cmake
find src/ugdk \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) -path "*/3D/*" | sort >> src/3d_module_src_list.cmake
find src/bulletworks \( -name "*.cc" -o -name "*.h" -o -name "*.th" -o -name "*.tcc" \) | sort >> src/3d_module_src_list.cmake
echo ")" >> src/3d_module_src_list.cmake
echo "Arquivo src/3d_module_src_list.txt criado com sucesso."

# LUA SOURCE
echo "SET(UGDK_LUA_SRC " > src/lua_src_list.cmake
find src/ugdk/script/languages/lua -name "*.cc" -o -name "*.h" | sort >> src/lua_src_list.cmake
echo ")" >> ./src/lua_src_list.cmake
echo "Arquivo src/lua_src_list.txt criado com sucesso."

# PYTHON SOURCE
echo "SET(UGDK_PYTHON_SRC " > src/py_src_list.cmake
find src/ugdk/script/languages/python -name "*.cc" -o -name "*.h" | sort >> src/py_src_list.cmake
echo ")" >> src/py_src_list.cmake
echo "Arquivo src/py_src_list.txt criado com sucesso."

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

