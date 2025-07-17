# CMakeLists.txt for cmath3 library
cmake_minimum_required(VERSION 3.10)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 项目定义
project(cmath3 C)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 查找源文件
file(GLOB_RECURSE SOURCES
    "source/*.c"
)

file(GLOB_RECURSE HEADERS
    "source/*.h"
    "types/*.h"
    "cmath3.h"
)

# 创建静态库
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# 设置包含目录
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/types
)

# 设置库输出名称
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "cmath3"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
