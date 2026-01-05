/**
 * @file html_context.h
 * @brief HTML Context API - 支持多实例的HTML处理
 *
 * 此文件提供了基于context的HTML处理API，支持同时加载和处理多个HTML文档。
 * 每个context代表一个独立的HTML文档实例。
 */

#ifndef __HTML_CONTEXT_H__
#define __HTML_CONTEXT_H__

#include "common/shared.h"
#include "renderer/r_local.h"
#include "layout.h"

// Forward declarations
typedef struct context context;
typedef struct _xmlNode xmlNode;
typedef struct _xmlDoc xmlDoc;

// ==================== Context生命周期管理 ====================

/**
 * @brief 创建HTML上下文
 * @return 新创建的HTML上下文，失败返回NULL
 */
context* html_context_create(int width,int height);

/**
 * @brief 销毁HTML上下文
 * @param ctx 要销毁的HTML上下文
 */
void html_context_destroy(context *ctx);

// ==================== HTML文档加载 ====================

/**
 * @brief 从文件加载HTML文档
 * @param ctx HTML上下文
 * @param filename HTML文件路径
 * @return 0成功，非0失败
 */
int html_context_load_from_file(context *ctx, const char *filename);

/**
 * @brief 从内存加载HTML文档
 * @param ctx HTML上下文
 * @param html_data HTML数据缓冲区
 * @param length 数据长度
 * @return 0成功，非0失败
 */
int html_context_load_from_memory(context *ctx, const char *html_data, size_t length);

/**
 * @brief 获取HTML文档
 * @param ctx HTML上下文
 * @return XML文档指针
 */
xmlDoc* html_context_get_document(context *ctx);

/**
 * @brief 获取文档根元素
 * @param ctx HTML上下文
 * @return XML根节点指针
 */
xmlNode* html_context_get_root_element(context *ctx);

// ==================== 布局系统 ====================

/**
 * @brief 获取布局上下文
 * @param ctx HTML上下文
 * @return 布局上下文指针
 */
lay_context* html_context_get_layout_context(context *ctx);

/**
 * @brief 设置布局上下文（用于外部集成）
 * @param ctx HTML上下文
 * @param layout_ctx 布局上下文
 */
void html_context_set_layout_context(context *ctx, lay_context *layout_ctx);

/**
 * @brief 运行布局计算
 * @param ctx HTML上下文
 */
void html_context_run_layout(context *ctx);

/**
 * @brief 获取节点到布局ID的映射表
 * @param ctx HTML上下文
 * @return 哈希表指针
 */
struct PZHashTable* html_context_get_node_layout_map(context *ctx);

// ==================== 更新和渲染 ====================

/**
 * @brief 更新HTML内容和布局
 * @param ctx HTML上下文
 * @param delta_time 时间增量（秒）
 */
void html_context_update_and_layout(context *ctx, float delta_time);

/**
 * @brief 渲染HTML文档
 * @param ctx HTML上下文
 */
void html_context_render(context *ctx);

/**
 * @brief 渲染HTML背景
 * @param ctx HTML上下文
 */
void html_context_render_background(context *ctx);

// ==================== 字体管理 ====================

/**
 * @brief 设置HTML渲染字体
 * @param ctx HTML上下文
 * @param font_path 字体文件路径
 * @param size 字体大小
 * @return true成功，false失败
 */
bool html_context_set_font(context *ctx, const char *font_path, DWORD size);

/**
 * @brief 清理HTML渲染资源
 * @param ctx HTML上下文
 */
void html_context_render_cleanup(context *ctx);

// ==================== 动画系统 ====================

/**
 * @brief 获取动画管理器
 * @param ctx HTML上下文
 * @return 动画管理器指针
 */
void* html_context_get_animation_manager(context *ctx);

/**
 * @brief 扫描并存储关键帧动画
 * @param ctx HTML上下文
 */
void html_context_scan_keyframes(context *ctx);

/**
 * @brief 重新应用所有动画
 * @param ctx HTML上下文
 */
void html_context_reapply_animations(context *ctx);

/**
 * @brief 更新动画（内部函数）
 * @param ctx HTML上下文
 * @param delta_time 时间增量
 */
void html_context_update_animations(context *ctx, float delta_time);

// ==================== DOM操作 ====================

/**
 * @brief 根据ID查找元素
 * @param ctx HTML上下文
 * @param id 元素ID
 * @return XML节点指针，未找到返回NULL
 */
xmlNode* html_context_get_element_by_id(context *ctx, const char *id);

/**
 * @brief 根据类名查找元素
 * @param ctx HTML上下文
 * @param class_name CSS类名
 * @param results 结果数组
 * @param max_results 最大结果数量
 * @return 找到的元素数量
 */
int html_context_get_elements_by_class_name(context *ctx, const char *class_name,
                                          xmlNode **results, int max_results);

/**
 * @brief 根据标签名查找元素
 * @param ctx HTML上下文
 * @param tag_name 标签名
 * @param results 结果数组
 * @param max_results 最大结果数量
 * @return 找到的元素数量
 */
int html_context_get_elements_by_tag_name(context *ctx, const char *tag_name,
                                         xmlNode **results, int max_results);

// ==================== 样式操作 ====================

/**
 * @brief 获取元素的样式
 * @param ctx HTML上下文
 * @param node XML节点
 * @return 样式指针
 */
void* html_context_get_node_style(context *ctx, xmlNode *node);

/**
 * @brief 应用CSS到布局
 * @param ctx HTML上下文
 * @param node XML节点
 * @param css CSS字符串
 */
void html_context_apply_css(context *ctx, xmlNode *node, const char *css);

// ==================== Keyframes管理 ====================

/**
 * @brief 获取存储的关键帧数量
 * @param ctx HTML上下文
 * @return 关键帧数量
 */
int html_context_get_keyframes_count(context *ctx);

/**
 * @brief 根据名称查找关键帧
 * @param ctx HTML上下文
 * @param name 动画名称
 * @param keyframes 输出的关键帧数组
 * @param max_keyframes 最大关键帧数量
 * @return 关键帧数量
 */
int html_context_find_keyframes(context *ctx, const char *name,
                                void *keyframes, int max_keyframes);

// ==================== CSS选择 ====================

/**
 * @brief 获取CSS选择上下文
 * @param ctx HTML上下文
 * @return CSS选择上下文指针
 */
void* html_context_get_css_select_context(context *ctx);

/**
 * @brief 获取CSS样式表
 * @param ctx HTML上下文
 * @return CSS样式表指针
 */
void* html_context_get_css_stylesheet(context *ctx);

// ==================== 实用函数 ====================

/**
 * @brief 打印布局信息（调试用）
 * @param ctx HTML上下文
 */
void html_context_print_layout_info(context *ctx,int depth);

/**
 * @brief 处理样式和脚本
 * @param ctx HTML上下文
 * @param node 根节点
 * @param depth 深度
 */
void html_context_process_styles_and_scripts(context *ctx, xmlNode *node, int depth);

#endif // __HTML_CONTEXT_H__
