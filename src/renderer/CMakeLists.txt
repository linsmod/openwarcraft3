# CMakeLists.txt for renderer library
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 项目定义
project(renderer C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 定义变量
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor)

# 编译选项
add_compile_definitions(GL_GLEXT_PROTOTYPES)

# 配置类型
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Debug配置
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -D_DEBUG")

# Release配置
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# stb 模块
set(STB_SOURCE_FILES
    stb/stb_truetype.c
)

set(STB_HEADER_FILES
    stb/stb_truetype.h
)

# m3 模块
set(M3_SOURCE_FILES
    m3/r_m3_load.c
)

set(M3_HEADER_FILES
    m3/r_m3.h
)

# mdx 模块
set(MDX_SOURCE_FILES
    mdx/r_mdx_interpolation.c
    mdx/r_mdx_load.c
    mdx/r_mdx_render.c
)

set(MDX_HEADER_FILES
    mdx/r_mdx.h
)

# w3m 模块
set(W3M_SOURCE_FILES
    w3m/r_war3map_cliffs.c
    w3m/r_war3map_ground.c
    w3m/r_war3map_utils.c
    w3m/r_war3map_water.c
    w3m/r_war3map.c
)

set(W3M_HEADER_FILES
    w3m/r_war3map.h
)

# renderer 主模块文件
set(RENDERER_SOURCE_FILES
    r_blp1.c
    r_blp2.c
    r_buffer.c
    r_dds.c
    r_draw.c
    r_ents.c
    r_fogofwar.c
    r_font.c
    r_main.c
    r_particles.c
    r_shader.c
    r_sysfont.c
    r_texture.c
)

set(RENDERER_HEADER_FILES
    r_local.h
)

# 创建动态库
add_library(${PROJECT_NAME} SHARED
    ${RENDERER_SOURCE_FILES}
    ${RENDERER_HEADER_FILES}
    ${STB_SOURCE_FILES}
    ${STB_HEADER_FILES}
    ${M3_SOURCE_FILES}
    ${M3_HEADER_FILES}
    ${MDX_SOURCE_FILES}
    ${MDX_HEADER_FILES}
    ${W3M_SOURCE_FILES}
    ${W3M_HEADER_FILES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 库目录
target_link_directories(${PROJECT_NAME} PRIVATE
)

find_package(SDL2 REQUIRED)
# 链接库
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2
    cmath3
)

# 设置库输出名称和版本
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "renderer"
    VERSION 1.0.0
    SOVERSION 1
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
