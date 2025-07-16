#!/bin/bash

# 默认构建类型
BUILD_TYPE=""

# 处理命令行参数
if [ $# -gt 0 ]; then
    case "$1" in
        debug|Debug)
            BUILD_TYPE="Debug"
            echo "Building Debug version..."
            cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DENABLE_ASAN=OFF -B build
            ;;
        release|Release)
            BUILD_TYPE="Release"
            echo "Building Release version..."
            cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -B build
            ;;
        *)
            echo "Unknown build type: $1"
            echo "Usage: $0 [debug|release]"
            exit 1
            ;;
    esac
    
    # 重新配置CMake以设置构建类型
    
else
    echo "No build type specified, using default..."
fi

# 执行构建命令
cmake --build build