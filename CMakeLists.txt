# CMakeLists.txt for openwarcraft3 (Linux)
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 项目定义
project(openwarcraft3 C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 定义变量
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# 编译选项
add_compile_definitions(GL_GLEXT_PROTOTYPES)

# 配置类型
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Debug配置
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -D_DEBUG")

# Release配置
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# 禁用异常处理和RTTI (C语言不需要这些，但保留为注释以与premake保持一致)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

# 查找源文件
file(GLOB_RECURSE SOURCES 
    "src/*.c"
)

file(GLOB_RECURSE HEADERS 
    "src/*.h"
    "src/*.inl"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/game
    src/game/api
    src/renderer
    src/server
    src/lib
    ${VENDOR_DIR}/stormlib/src
    ${VENDOR_DIR}/sdl/include
)

# 库目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VENDOR_DIR}/stormlib/build
    ${VENDOR_DIR}/sdl/build
)

# 链接库 (Linux特定)
target_link_libraries(${PROJECT_NAME}
    dl
    GL
    pthread
    X11
    m
    SDL2
    jpeg
    storm
)

# 设置调试工作目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)