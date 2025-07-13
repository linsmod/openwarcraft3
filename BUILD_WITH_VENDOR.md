# 使用Vendor依赖构建OpenWarcraft3 (Linux)

本文档介绍如何在Linux系统上使用vendor目录中的依赖库构建OpenWarcraft3，而不是使用系统安装的库。

## 前提条件

1. 确保已安装必要的构建工具：
   ```bash
   sudo apt update
   sudo apt install build-essential cmake git
   ```

2. 确保已初始化和更新Git子模块：
   ```bash
   git submodule update --init --recursive
   ```

## 构建步骤

### 1. 构建Vendor依赖库

首先构建vendor目录中的依赖库：

```bash
./build_vendor.sh
```

这个脚本会：
- 构建StormLib静态库 (`vendor/stormlib/build/libstorm.a`)
- 构建SDL2静态库 (`vendor/sdl/build/libSDL2.a`)

### 2. 配置项目使用Vendor库

项目已经配置为使用vendor库，主要修改包括：

- **CMakeLists.txt** 已更新为链接vendor中的静态库
- 移除了对系统SDL2和storm库的依赖
- 使用绝对路径指向vendor构建的库文件

### 3. 构建项目

使用标准的构建脚本：

```bash
# 构建Release版本
./build.sh release

# 或构建Debug版本
./build.sh debug
```

## 验证构建

构建完成后，可以通过以下方式验证是否使用了vendor库：

```bash
# 检查可执行文件依赖
ldd build/openwarcraft3

# 应该看不到libSDL2-2.0.so.0和libstorm.so的依赖
# 只应看到系统库依赖
```

## 故障排除

### 1. 如果构建失败

检查vendor库是否正确构建：
```bash
ls -la vendor/stormlib/build/libstorm.a
ls -la vendor/sdl/build/libSDL2.a
```

### 2. 如果缺少依赖

确保所有子模块已正确初始化：
```bash
git submodule status
```

### 3. 清理并重新构建

如果需要清理所有构建：
```bash
rm -rf build vendor/stormlib/build vendor/sdl/build
./build_vendor.sh
./build.sh release
```

## 构建脚本说明

- **build_vendor.sh**: 构建vendor依赖库
- **build.sh**: 构建主项目，支持debug/release配置
- **CMakeLists.txt**: 已配置为使用vendor静态库

## 注意事项

- 使用静态库可以避免运行时依赖问题
- 构建的vendor库是Release版本，如需Debug版本请修改build_vendor.sh
- 如果需要更新vendor库版本，请更新相应的Git子模块
