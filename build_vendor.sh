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
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DSTORM_USE_BUNDLED_LIBRARIES=ON
cmake --build build --config Release
cd ../..

# 构建SDL2
echo "Building SDL2..."
cd vendor/sdl
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release -DfPIC -DBUILD_SHARED_LIBS=ON -DSDL_STATIC=OFF -DSDL_SHARED=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build --config Release
cd ../..

echo "Vendor dependencies built successfully!"
