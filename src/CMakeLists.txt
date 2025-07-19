# CMakeLists.txt for openwarcraft3 (Linux)
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 项目定义
project(openwarcraft3 C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
option(ENABLE_ASAN "Enable AddressSanitizer for memory error detection" OFF)
if(ENABLE_ASAN)
    # 添加编译器标志
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    
    # 添加链接器标志
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

# 定义变量
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../vendor)

# 编译选项
add_compile_definitions(GL_GLEXT_PROTOTYPES)

# 配置类型
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Debug配置
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -D_DEBUG -DDEBUG_JASS")

# Release配置
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# 禁用异常处理和RTTI (C语言不需要这些，但保留为注释以与premake保持一致)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

# 添加子目录
add_subdirectory(cmath3)
add_subdirectory(renderer)
add_subdirectory(game)

# common 模块
set(COMMON_SOURCE_FILES
    common/cmd.c
    common/common.c
    common/macos.c
    common/main.c
    common/msg.c
    common/net.c
    common/parser.c
    common/routing.c
    common/sheet.c
    common/world.c
)

set(COMMON_HEADER_FILES
    common/cmodel.h
    common/common.h
    common/mapinfo.h
    common/net.h
    common/shared.h
)

# client 模块
set(CLIENT_SOURCE_FILES
    client/cl_console.c
    client/cl_fx.c
    client/cl_input.c
    client/cl_main.c
    client/cl_parse.c
    client/cl_scrn.c
    client/cl_tent.c
    client/cl_view.c
    client/keys.c
)

set(CLIENT_HEADER_FILES
    client/client.h
    client/keys.h
    client/renderer.h
)

# server 模块
set(SERVER_SOURCE_FILES
    server/sv_ents.c
    server/sv_game.c
    server/sv_init.c
    server/sv_main.c
    server/sv_parse.c
    server/sv_send.c
    server/sv_user.c
    server/sv_world.c
)

set(SERVER_HEADER_FILES
    server/game.h
    server/server.h
)

# 创建可执行文件
add_executable(openwarcraft3
    ${COMMON_SOURCE_FILES}
    ${CLIENT_SOURCE_FILES}
    ${SERVER_SOURCE_FILES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    game
    game/api
    server
    lib
    ${VENDOR_DIR}/stormlib/src
    ${VENDOR_DIR}/sdl/include
)

find_package(SDL2 REQUIRED)

# 链接库
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
)

# 库目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VENDOR_DIR}/stormlib/build
    ${CMAKE_CURRENT_SOURCE_DIR}/cmath3/build
    # ${VENDOR_DIR}/sdl/build
)

# 链接库 (Linux特定)
target_link_libraries(${PROJECT_NAME}
    dl
    GL
    pthread
    # X11
    m
    jpeg
    # ${CMAKE_CURRENT_SOURCE_DIR}/cmath3/build/libcmath3.a
    cmath3
    renderer
    game
    # SDL2
    # storm
    # ${VENDOR_DIR}/sdl/build/libSDL2.so
    ${VENDOR_DIR}/stormlib/build/libstorm.so
)

# 设置调试工作目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
