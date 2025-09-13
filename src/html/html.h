#include "common/shared.h"
#include "renderer/r_local.h"

int html_init(LPCSTR filename);
int html_destroy();
void html_update_frame();

// HTML渲染字体管理
bool html_render_set_font(LPCSTR font_path, DWORD size) ;
void html_render_cleanup();