#ifndef __UI_CONTAINER_H__
#define __UI_CONTAINER_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_button.h"
#include "ui_text.h"

// UI 容器子组件类型
typedef enum {
    UI_CONTAINER_ITEM_TYPE_BUTTON,
    UI_CONTAINER_ITEM_TYPE_TEXT
} ui_container_item_type_t;

// UI 容器子组件
typedef struct {
    ui_container_item_type_t type;
    void *widget;  // 指向 ui_button_t 或 ui_text_t 的指针
} ui_container_item_t;

// UI 容器配置
typedef struct {
    float x;                          // 容器 X 位置
    float y;                          // 容器 Y 位置
    float width;                      // 容器宽度
    float height;                     // 容器高度
    COLOR32 bg_color;                 // 背景色
    COLOR32 border_color;             // 边框色
    float border_width;               // 边框宽度
    bool visible;                     // 是否可见 (默认 true)
    int max_items;                   // 最大子组件数量
} ui_container_config_t;

// UI 容器状态
typedef struct {
    ui_container_config_t config;
    ui_container_item_t *items;        // 子组件数组
    int item_count;                  // 子组件数量
    canvas2d_context_t *ctx;
} ui_container_t;

// 创建默认容器配置
ui_container_config_t UIContainer_GetDefaultConfig(void);

// 初始化 UI 容器
int UIContainer_Init(ui_container_t *container, const ui_container_config_t *config, canvas2d_context_t *ctx);

// 设置容器位置
void UIContainer_SetPosition(ui_container_t *container, float x, float y);

// 设置容器大小
void UIContainer_SetSize(ui_container_t *container, float width, float height);

// 设置容器背景色
void UIContainer_SetBgColor(ui_container_t *container, COLOR32 color);

// 设置容器边框色
void UIContainer_SetBorderColor(ui_container_t *container, COLOR32 color);

// 添加按钮子组件
int UIContainer_AddButton(ui_container_t *container, ui_button_t *button);

// 添加文本子组件
int UIContainer_AddText(ui_container_t *container, ui_text_t *text);

// 移除子组件（按索引）
void UIContainer_RemoveItem(ui_container_t *container, int index);

// 清空所有子组件
void UIContainer_ClearItems(ui_container_t *container);

// 设置可见性
void UIContainer_SetVisible(ui_container_t *container, bool visible);

// 获取可见性
bool UIContainer_IsVisible(const ui_container_t *container);

// 检查点是否在容器区域内
bool UIContainer_IsPointInContainer(const ui_container_t *container, float x, float y);

// 处理鼠标移动
bool UIContainer_HandleMouseMove(ui_container_t *container, float x, float y);

// 处理鼠标点击
bool UIContainer_HandleMouseClick(ui_container_t *container, float x, float y, bool down);

// 更新容器
void UIContainer_Update(ui_container_t *container, int msec);

// 渲染容器
void UIContainer_Render(ui_container_t *container);

// 清理容器
void UIContainer_Shutdown(ui_container_t *container);

#endif // __UI_CONTAINER_H__
