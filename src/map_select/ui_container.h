#ifndef __UI_CONTAINER_H__
#define __UI_CONTAINER_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"

// UI 容器组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件（包含bg_color）
    COLOR32 border_color;             // 边框色
    float border_width;               // 边框宽度
    int max_children;                 // 最大子组件数量
} ui_container_t;

// 创建默认容器配置
ui_container_t* UIContainer_Create(float x, float y, float width, float height,
                                  COLOR32 bg_color, COLOR32 border_color, canvas2d_context_t *ctx);

// 初始化 UI 容器
int UIContainer_Init(ui_container_t *container, canvas2d_context_t *ctx);

// 设置容器位置
void UIContainer_SetPosition(ui_container_t *container, float x, float y);

// 设置容器大小
void UIContainer_SetSize(ui_container_t *container, float width, float height);

// 设置容器背景色
void UIContainer_SetBgColor(ui_container_t *container, COLOR32 color);

// 设置容器边框色
void UIContainer_SetBorderColor(ui_container_t *container, COLOR32 color);

// 添加子组件
int UIContainer_AddChild(ui_container_t *container, ui_component_t *child);

// 移除子组件
bool UIContainer_RemoveChild(ui_container_t *container, ui_component_t *child);

// 按索引移除子组件
bool UIContainer_RemoveChildByIndex(ui_container_t *container, int index);

// 清空所有子组件
void UIContainer_ClearChildren(ui_container_t *container);

// 获取子组件数量
int UIContainer_GetChildCount(ui_container_t *container);

// 获取子组件
ui_component_t* UIContainer_GetChild(ui_container_t *container, int index);

// 更新容器
void UIContainer_Update(ui_container_t *container, int msec);

// 渲染容器
void UIContainer_Render(ui_container_t *container);

// 清理容器
void UIContainer_Shutdown(ui_container_t *container);

// 销毁容器
void UIContainer_Destroy(ui_container_t *container);

#endif // __UI_CONTAINER_H__
