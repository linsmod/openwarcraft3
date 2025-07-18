#!/bin/bash

# 默认构建类型
export BUILD_TYPE="${BUILD_TYPE:-Debug}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
m()
{
    local p="${1:-src}"
    echo "Building in directory: $p"
    cd "$p"
    # rm -rf build
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DENABLE_ASAN=ON -B "build" &&
    cmake --build build &&
    python $SCRIPT_DIR/combine_compile_commands.py

    if [ $? -ne 0 ]; then
        echo "Build failed."
        exit 1
    fi
    cp $SCRIPT_DIR/src/build/openwarcraft3 $SCRIPT_DIR/build/openwarcraft3
    cd $SCRIPT_DIR
}
home(){
    cd $SCRIPT_DIR
}
m

