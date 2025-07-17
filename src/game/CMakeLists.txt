# CMakeLists.txt for game library
cmake_minimum_required(VERSION 3.10)

# 启用compile_commands.json生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 项目定义
project(game C)

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

# 定义源文件
set(SOURCE_FILES
    g_ai.c
    g_commands.c
    g_events.c
    g_items.c
    g_main.c
    g_metadata.c
    g_monster.c
    g_pathing.c
    g_phys.c
    g_save.c
    g_spawn.c
    g_utils.c
    m_tree.c
    m_unit.c
    parser.c
)

set(HEADER_FILES
    g_local.h
    g_metadata.h
    g_shared.h
    g_unitdata.h
    parser.h
)

set(API_SOURCE_FILES
    api/api_module.c
)

set(UI_SOURCE_FILES
    ui/ui_fdf.c
    ui/ui_init.c
    ui/ui_write.c
)

set(JASS_SOURCE_FILES
    jass/jass_parser.c
    jass/vm_compiler.c
    jass/vm_main.c
)

set(SKILLS_SOURCE_FILES
    skills/s_attack.c
    skills/s_build.c
    skills/s_cancel.c
    skills/s_devotionaura.c
    skills/s_goldmine.c
    skills/s_harvest_lumber.c
    skills/s_holdpos.c
    skills/s_holylight.c
    skills/s_militia.c
    skills/s_move.c
    skills/s_patrol.c
    skills/s_repair.c
    skills/s_selectskill.c
    skills/s_skills.c
    skills/s_stop.c
    skills/s_train.c
)

set(HUD_SOURCE_FILES
    hud/ui_quests.c
    hud/ui_console.c
    hud/ui_log.c
)

set(API_HEADER_FILES
    api/api_camera.h
    api/api_destructable.h
    api/api_cinefilter.h
    api/api_effect.h
    api/api_group.h
    api/api_item.h
    api/api_leaderboard.h
    api/api_macros.h
    api/api_math.h
    api/api_misc.h
    api/api_player.h
    api/api_quest.h
    api/api_sound.h
    api/api_trigger.h
    api/api_unit.h
)

set(JASS_HEADER_FILES
    jass/jass_parser.h
    jass/vm_bytecode.h
    jass/vm_public.h
)

set(SKILLS_HEADER_FILES
    skills/s_skills.h
)

# 创建动态库
# 创建动态库
add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${API_SOURCE_FILES}
    ${UI_SOURCE_FILES}
    ${JASS_SOURCE_FILES}
    ${SKILLS_SOURCE_FILES}
    ${HUD_SOURCE_FILES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/api
)

# 库目录
target_link_directories(${PROJECT_NAME} PRIVATE
    ${VENDOR_DIR}/stormlib/build
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../src/cmath3/build
)

# find_package(SDL2 REQUIRED)

# 链接库
# target_link_libraries(${PROJECT_NAME}
#     cmath3
#     libstorm.so
# )

# 设置库输出名称和版本
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "game"
    VERSION 1.0.0
    SOVERSION 1
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
