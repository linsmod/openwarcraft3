#include "debug_overlay_scene.h"
#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "../html/html.h"
#include <libxml/tree.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// 前向声明
static int DebugOverlay_Init(scene_t *scene, const scene_params_t *params);
static void DebugOverlay_Shutdown(scene_t *scene);
static scene_transition_t* DebugOverlay_Update(scene_t *scene, int msec);

// 创建调试覆盖层场景
scene_t* DebugOverlayScene_Create(void) {
    scene_t *scene = (scene_t*)malloc(sizeof(scene_t));
    if (!scene) {
        printf("DebugOverlayScene: Failed to allocate memory\n");
        return NULL;
    }

    memset(scene, 0, sizeof(scene_t));
    scene->name = "DebugOverlay";
    scene->state = SCENE_STATE_UNINITIALIZED;

    // 设置场景函数指针
    scene->init = DebugOverlay_Init;
    scene->shutdown = DebugOverlay_Shutdown;
    scene->update = DebugOverlay_Update;
    scene->render = DebugOverlayScene_Render;

    printf("DebugOverlayScene: Created\n");
    return scene;
}

// 初始化调试覆盖层场景
static int DebugOverlay_Init(scene_t *scene, const scene_params_t *params) {
    (void)params;

    printf("DebugOverlayScene: Initialized\n");
    return 0;
}

// 关闭调试覆盖层场景
static void DebugOverlay_Shutdown(scene_t *scene) {
    (void)scene;

    printf("DebugOverlayScene: Shutdown\n");
}

// 更新调试覆盖层场景（不需要更新逻辑）
static scene_transition_t* DebugOverlay_Update(scene_t *scene, int msec) {
    (void)scene;
    (void)msec;

    // 不产生任何场景跳转请求
    return NULL;
}

// 渲染调试覆盖层场景
void DebugOverlayScene_Render(scene_t *scene) {
    if (!scene || !scene->manager) return;

    scene_manager_t *mgr = scene->manager;

    // 如果有当前场景且有命中的组件
    if (mgr->current_scene && mgr->mouse_target) {
        ui_component_t *target = mgr->mouse_target;

        float x, y, width, height;
        UIComponent_GetComputedRectXywh(target, &x, &y, &width, &height);
        // 获取画布上下文
        canvas2d_context_t *ctx = mgr->current_scene->canvas_ctx;
        if (!ctx) return;

        // 绘制红色边框表示命中的组件
        canvas2d_set_stroke_style(ctx, MAKE(COLOR32, 255, 0, 0, 255)); // 红色
        canvas2d_set_line_width(ctx, 2.0f);
        canvas2d_stroke_rect(ctx, x, y, width, height);

        // 绘制组件类型名称
        const char *type_name = UIComponent_GetTypeName(target->type);
        
        // 如果是HTML节点，尝试显示标签名
        char type_info[256];
        if (target->type == UI_COMPONENT_TYPE_HTML_NODE || target->type == UI_COMPONENT_TYPE_HTML_DOC) {
            // 获取XML节点
            if (target->xml_node) {
                xmlNode *xml_node = (xmlNode *)target->xml_node;
                if (xml_node && xml_node->name) {
                    snprintf(type_info, sizeof(type_info), "HTML_%s", (char*)xml_node->name);
                } else {
                    strncpy(type_info, type_name ? type_name : "UNKNOWN", sizeof(type_info) - 1);
                }
            } else {
                strncpy(type_info, type_name ? type_name : "UNKNOWN", sizeof(type_info) - 1);
            }
        } else {
            strncpy(type_info, type_name ? type_name : "UNKNOWN", sizeof(type_info) - 1);
        }
        
        if (type_name || type_info[0] != '\0') {
            char info_text[512];
            snprintf(info_text, sizeof(info_text),
                "[%s] (%.0f, %.0f) %.0fx%.0f",
                type_info, x, y, width, height);

            // 设置文字样式
            canvas2d_set_font_size(ctx, 14.0f);
            canvas2d_set_fill_style(ctx, MAKE(COLOR32, 255, 255, 0, 255)); // 黄色
            
            // 在组件上方显示信息
            canvas2d_fill_text(ctx, info_text, x, y - 20.0f);
        }
    }
}
