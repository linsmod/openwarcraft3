#!/bin/bash

# 构建vendor目录中的依赖库
set -e

echo "Building vendor dependencies..."

# 创建构建目录
mkdir -p vendor/stormlib/build
mkdir -p vendor/sdl/build

# 构建StormLib
echo "Building StormLib..."
cd vendor/stormlib
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DSTORM_USE_BUNDLED_LIBRARIES=ON
cmake --build build --config Release
cd ../..

# 构建SDL2
echo "Building SDL2..."
cd vendor/sdl
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DSDL_STATIC=ON -DSDL_SHARED=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build --config Release
cd ../..

echo "Vendor dependencies built successfully!"
