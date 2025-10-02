# CMakeLists.txt for openwarcraft3 (Linux)
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置自定义库路径
set(CUSTOM_LIB_DIR "/home/wulin/netsurf_libs/projects/inst-x86_64-linux-gnu")
set(ENV{PKG_CONFIG_PATH} "${CUSTOM_LIB_DIR}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
list(APPEND CMAKE_PREFIX_PATH "${CUSTOM_LIB_DIR}")
list(APPEND CMAKE_LIBRARY_PATH "${CUSTOM_LIB_DIR}/lib")
list(APPEND CMAKE_INCLUDE_PATH "${CUSTOM_LIB_DIR}/include")

# 查找pkg-config
find_package(PkgConfig REQUIRED)

# 使用pkgconfig查找netsurf相关库
pkg_check_modules(HUBBUB REQUIRED libhubbub)
pkg_check_modules(LIBCSS REQUIRED libcss)
pkg_check_modules(PARSERUTILS REQUIRED libparserutils)
pkg_check_modules(WAPCAPLET REQUIRED libwapcaplet)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 确保输出目录存在
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# 根据构建类型设置不同的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG _DEBUG)
    add_compile_options(-O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
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
    "src/libs/*.c"
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
    ${CUSTOM_LIB_DIR}/include
    ${HUBBUB_INCLUDE_DIRS}
    ${LIBCSS_INCLUDE_DIRS}
    ${PARSERUTILS_INCLUDE_DIRS}
    ${WAPCAPLET_INCLUDE_DIRS}
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
    ${HUBBUB_LIBRARIES}
    ${LIBCSS_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${PARSERUTILS_LIBRARIES}
    ${WAPCAPLET_LIBRARIES}
)

# 设置链接目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CUSTOM_LIB_DIR}/lib
    ${HUBBUB_LIBRARY_DIRS}
    ${LIBCSS_LIBRARY_DIRS}
    ${PARSERUTILS_LIBRARY_DIRS}
    ${WAPCAPLET_LIBRARY_DIRS}
)

# 设置调试工作目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(TARGETS cmath3 game renderer
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)