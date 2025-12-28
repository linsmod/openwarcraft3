#include "common/shared.h"
#include "renderer/r_local.h"

// Forward declarations
typedef struct context context;
typedef struct _xmlNode xmlNode;

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