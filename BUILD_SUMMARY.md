# OpenWarcraft3 使用Vendor依赖构建总结

## 构建成功 ✅

项目已成功配置为使用vendor目录中的依赖库，而不是系统安装的库。

## 构建结果

- **可执行文件**: `build/openwarcraft3` (3.8MB)
- **构建状态**: 成功
- **依赖验证**: 没有SDL2和StormLib的动态库依赖，证明使用了静态链接

## 使用的Vendor库

1. **StormLib** (`vendor/stormlib/build/libstorm.a`)
   - 使用 `-DSTORM_USE_BUNDLED_LIBRARIES=ON` 构建
   - 包含所有必要的依赖（zlib, bzip2等）

2. **SDL2** (`vendor/sdl/build/libSDL2.a`)
   - 使用 `-DSDL_STATIC=ON -DSDL_SHARED=OFF` 构建
   - 静态链接，无运行时依赖

## 关键修改

### 1. CMakeLists.txt 修改
- 使用 `project(openwarcraft3 C CXX)` 支持C++链接
- 链接vendor静态库：
  - `${VENDOR_DIR}/sdl/build/libSDL2.a`
  - `${VENDOR_DIR}/stormlib/build/libstorm.a`
- 添加必要的系统库：z, jpeg, stdc++

### 2. 构建脚本
- **build_vendor.sh**: 构建vendor依赖
- **build.sh**: 构建主项目

## 构建流程

### 完整构建命令
```bash
# 1. 初始化子模块（只需一次）
git submodule update --init --recursive

# 2. 构建vendor依赖
./build_vendor.sh

# 3. 构建项目
./build.sh release
```

### 验证构建
```bash
# 检查可执行文件
ls -la build/openwarcraft3

# 检查依赖（不应看到libSDL2-2.0.so.0或libstorm.so）
ldd build/openwarcraft3
```

## 文件结构
```
openwarcraft3/
├── build/                    # 主项目构建目录
├── vendor/
│   ├── stormlib/
│   │   └── build/
│   │       └── libstorm.a   # StormLib静态库
│   └── sdl/
│       └── build/
│           └── libSDL2.a    # SDL2静态库
├── build_vendor.sh          # vendor依赖构建脚本
├── build.sh                 # 主项目构建脚本
└── BUILD_WITH_VENDOR.md     # 详细构建文档
```

## 优势

1. **无运行时依赖**: 不依赖系统安装的SDL2和StormLib
2. **可移植性**: 可执行文件可在任何Linux系统上运行
3. **版本控制**: 使用特定版本的依赖库
4. **一致性**: 确保所有开发者使用相同的库版本

## 故障排除

如果遇到问题：
1. 清理并重新构建：`rm -rf build vendor/*/build && ./build_vendor.sh && ./build.sh release`
2. 检查子模块：`git submodule status`
3. 验证库文件：`ls -la vendor/*/build/*.a`
