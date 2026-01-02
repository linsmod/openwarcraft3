#include "common/shared.h"
#include "renderer/r_local.h"

#include "layout.h"

// Forward declarations
typedef struct context context;
typedef struct _xmlNode xmlNode;
typedef struct _xmlDoc xmlDoc;

// ========================================
// 旧版全局API（向后兼容，内部使用context 0）
// ========================================

int html_init(LPCSTR filename);
int html_destroy();
void html_update_and_layout(float delta_time, int page_index);
void html_render();

// HTML渲染字体管理
bool html_render_set_font(LPCSTR font_path, DWORD size) ;
void html_render_cleanup();

// Keyframes扫描和存储
void html_scan_and_store_keyframes(void);
void html_reapply_all_animations(void);

// Animation属性应用（内部函数）
void apply_animation_attribute(context *ctx, xmlNode *node, const char *animation_value);

// ========================================
// 新版Context API（推荐使用）
// ========================================

/**
 * @brief 创建HTML上下文
 * @return 新的HTML上下文，失败返回NULL
 */
context* html_context_create(void);

/**
 * @brief 销毁HTML上下文
 * @param ctx HTML上下文
 */
void html_context_destroy(context *ctx);

/**
 * @brief 从文件加载HTML
 * @param ctx HTML上下文
 * @param filename 文件名
 * @return 0成功，非0失败
 */
int html_context_load_file(context *ctx, const char *filename);

/**
 * @brief 从内存加载HTML
 * @param ctx HTML上下文
 * @param html_data HTML数据
 * @param length 数据长度
 * @return 0成功，非0失败
 */
int html_context_load_memory(context *ctx, const char *html_data, size_t length);

/**
 * @brief 更新和布局
 * @param ctx HTML上下文
 * @param delta_time 时间增量（秒）
 */
void html_context_update(context *ctx, float delta_time);

/**
 * @brief 渲染HTML
 * @param ctx HTML上下文
 */
void html_context_render(context *ctx);

/**
 * @brief 设置字体
 * @param ctx HTML上下文
 * @param font_path 字体路径
 * @param size 字体大小
 * @return true成功
 */
bool html_context_set_font(context *ctx, const char *font_path, DWORD size);

/**
 * @brief 扫描关键帧
 * @param ctx HTML上下文
 */
void html_context_scan_keyframes(context *ctx);

/**
 * @brief 重新应用动画
 * @param ctx HTML上下文
 */
void html_context_reapply_animations(context *ctx);

/**
 * @brief 获取文档
 * @param ctx HTML上下文
 * @return XML文档
 */
xmlDoc* html_context_get_document(context *ctx);

/**
 * @brief 获取根元素
 * @param ctx HTML上下文
 * @return 根节点
 */
xmlNode* html_context_get_root(context *ctx);

/**
 * @brief 获取布局上下文
 * @param ctx HTML上下文
 * @return 布局上下文
 */
lay_context* html_context_get_layout(context *ctx);

/**
 * @brief 根据ID查找元素
 * @param ctx HTML上下文
 * @param id 元素ID
 * @return 元素节点
 */
xmlNode* html_context_find_by_id(context *ctx, const char *id);