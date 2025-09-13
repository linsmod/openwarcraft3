# CMakeLists.txt for openwarcraft3 (Linux)
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 确保输出目录存在
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# add_compile_options(-Wno-unused-parameter -Wno-sign-compare)

# 根据构建类型设置不同的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 调试版本设置
    add_compile_definitions(DEBUG _DEBUG)
    add_compile_options(-O0)
    
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # 发布版本设置
    add_compile_definitions(NDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
    
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # 带调试信息的发布版本
    add_compile_definitions(NDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -g -DNDEBUG")
    
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

option(ENABLE_ASAN "Enable AddressSanitizer for memory error detection" OFF)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# 编译选项
add_compile_definitions(GL_GLEXT_PROTOTYPES)

# 项目定义
project(openwarcraft3 C)

# 查找源文件
file(GLOB SOURCES 
    "src/common/*.c"
    "src/client/*.c"
    "src/server/*.c"
    "src/html/*.c"
)

file(GLOB_RECURSE HEADERS 
    "src/*.h"
)

find_package(LibXml2 REQUIRED)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 包含目录
include_directories(
    src
    src/game
    src/game/api
    src/renderer
    src/server
    src/lib
    ${LIBXML2_INCLUDE_DIR}
)

file(GLOB_RECURSE CMATH3_SRCS "src/cmath3/*.c")
file(GLOB_RECURSE GAME_SRCS "src/game/*.c")
file(GLOB_RECURSE RENDERER_SRCS "src/renderer/*.c" "src/canvas2d/*.c")

# 创建共享库并设置输出目录
add_library(cmath3 SHARED ${CMATH3_SRCS})
add_library(game SHARED ${GAME_SRCS})
add_library(renderer SHARED ${RENDERER_SRCS})

target_link_libraries(renderer
    GL
    jpeg
)

# 链接库 (Linux特定)
target_link_libraries(${PROJECT_NAME}
    dl
    GL
    pthread
    X11
    m
    jpeg
    SDL2
    storm
    cmath3
    game
    renderer
    hubbub
    css
    xml2
    parserutils
    wapcaplet
)

# 设置调试工作目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data"
)

# 安装规则 - 同时安装可执行文件和库
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(TARGETS cmath3 game renderer
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin  # Windows上的DLL
    ARCHIVE DESTINATION lib  # 静态库
)