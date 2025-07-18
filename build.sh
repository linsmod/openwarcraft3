#!/bin/bash

# 默认构建类型
export BUILD_TYPE="${BUILD_TYPE:-Debug}"
script_path=$(pwd)
m()
{
    local p="${1:-src}"
    echo "Building in directory: $p"
    cd "$p"
    # rm -rf build
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DENABLE_ASAN=ON -B "build" &&
    cmake --build build
    cd $script_path
}
home(){
    cd $script_path
}
m
