#!/bin/bash
# Build script for openwarcraft3

usage() {
    echo "Usage: $0 [--debug] [--clangd]"
    echo "Options:"
    echo "  --debug    Build debug version"
    echo "  --clangd   Generate compile_commands.json for clangd"
    exit 1
}

# Parse arguments
DEBUG=false
CLANGD=false
while [[ $# -gt 0 ]]; do
    case "$1" in
        --debug)
            DEBUG=true
            shift
            ;;
        --clangd)
            CLANGD=true
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done

cd ~/openwarcraft3

# 生成compile_commands.json（如果需要）
if $CLANGD; then
    echo "Generating compile_commands.json for clangd..."
    ./tools/bin/premake5 --scripts=. --file=compile_commands.lua compile_commands
fi

# 生成构建文件
./tools/bin/premake5 cmake
./tools/bin/premake5 gmake2

# 执行构建
if $DEBUG; then
    echo "Building debug version..."
    cd build/gmake2 && make config=debug_x86_64
else
    echo "Building release version..."
    cd build/gmake2 && make config=release_x86_64
fi

cd ~/openwarcraft3