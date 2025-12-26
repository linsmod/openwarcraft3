#include "ui_container.h"
#include <string.h>
#include <stdio.h>

// 创建默认容器配置
ui_container_config_t UIContainer_GetDefaultConfig(void) {
    ui_container_config_t config = {
        .x = 0.0f,
        .y = 0.0f,
        .width = 100.0f,
        .height = 100.0f,
        .bg_color = {40, 40, 50, 255},
        .border_color = {200, 200, 200, 255},
        .border_width = 1.0f,
        .visible = true,
        .max_items = 10
    };
    return config;
}

// 初始化 UI 容器
int UIContainer_Init(ui_container_t *container, const ui_container_config_t *config, canvas2d_context_t *ctx) {
    if (!container || !config || !ctx) {
        return -1;
    }

    memset(container, 0, sizeof(ui_container_t));
    container->config = *config;
    container->ctx = ctx;
    container->item_count = 0;

    // 分配子组件数组
    if (config->max_items > 0) {
        container->items = (ui_container_item_t *)malloc(sizeof(ui_container_item_t) * config->max_items);
        if (!container->items) {
            printf("Failed to allocate memory for container items\n");
            return -1;
        }
        memset(container->items, 0, sizeof(ui_container_item_t) * config->max_items);
    }

    printf("UIContainer initialized: pos=(%.1f,%.1f), size=(%.1fx%.1f), max_items=%d\n",
           config->x, config->y, config->width, config->height, config->max_items);

    return 0;
}

// 设置容器位置
void UIContainer_SetPosition(ui_container_t *container, float x, float y) {
    if (!container) return;
    container->config.x = x;
    container->config.y = y;
}

// 设置容器大小
void UIContainer_SetSize(ui_container_t *container, float width, float height) {
    if (!container) return;
    container->config.width = width;
    container->config.height = height;
}

// 设置容器背景色
void UIContainer_SetBgColor(ui_container_t *container, COLOR32 color) {
    if (!container) return;
    container->config.bg_color = color;
}

// 设置容器边框色
void UIContainer_SetBorderColor(ui_container_t *container, COLOR32 color) {
    if (!container) return;
    container->config.border_color = color;
}

// 添加按钮子组件
int UIContainer_AddButton(ui_container_t *container, ui_button_t *button) {
    if (!container || !button) return -1;
    if (container->item_count >= container->config.max_items) return -1;

    ui_container_item_t *item = &container->items[container->item_count];
    item->type = UI_CONTAINER_ITEM_TYPE_BUTTON;
    item->widget = button;
    container->item_count++;

    return container->item_count - 1;
}

// 添加文本子组件
int UIContainer_AddText(ui_container_t *container, ui_text_t *text) {
    if (!container || !text) return -1;
    if (container->item_count >= container->config.max_items) return -1;

    ui_container_item_t *item = &container->items[container->item_count];
    item->type = UI_CONTAINER_ITEM_TYPE_TEXT;
    item->widget = text;
    container->item_count++;

    return container->item_count - 1;
}

// 移除子组件（按索引）
void UIContainer_RemoveItem(ui_container_t *container, int index) {
    if (!container || index < 0 || index >= container->item_count) return;

    // 将后面的项前移
    for (int i = index; i < container->item_count - 1; i++) {
        container->items[i] = container->items[i + 1];
    }
    container->item_count--;
}

// 清空所有子组件
void UIContainer_ClearItems(ui_container_t *container) {
    if (!container) return;
    container->item_count = 0;
}

// 设置可见性
void UIContainer_SetVisible(ui_container_t *container, bool visible) {
    if (!container) return;
    container->config.visible = visible;
}

// 获取可见性
bool UIContainer_IsVisible(const ui_container_t *container) {
    if (!container) return false;
    return container->config.visible;
}

// 检查点是否在容器区域内
bool UIContainer_IsPointInContainer(const ui_container_t *container, float x, float y) {
    if (!container) return false;
    const ui_container_config_t *cfg = &container->config;
    return x >= cfg->x && x < cfg->x + cfg->width &&
           y >= cfg->y && y < cfg->y + cfg->height;
}

// 处理鼠标移动
bool UIContainer_HandleMouseMove(ui_container_t *container, float x, float y) {
    if (!container || !container->config.visible) return false;
    if (!UIContainer_IsPointInContainer(container, x, y)) return false;

    bool handled = false;
    // 传递鼠标移动事件给所有子组件
    for (int i = 0; i < container->item_count; i++) {
        ui_container_item_t *item = &container->items[i];
        if (item->type == UI_CONTAINER_ITEM_TYPE_BUTTON) {
            ui_button_t *button = (ui_button_t *)item->widget;
            if (UIButton_HandleMouseMove(button, x, y)) {
                handled = true;
            }
        }
    }
    return handled;
}

// 处理鼠标点击
bool UIContainer_HandleMouseClick(ui_container_t *container, float x, float y, bool down) {
    if (!container || !container->config.visible) return false;
    if (!UIContainer_IsPointInContainer(container, x, y)) return false;

    bool handled = false;
    // 传递鼠标点击事件给所有子组件（倒序遍历，先处理上层组件）
    for (int i = container->item_count - 1; i >= 0; i--) {
        ui_container_item_t *item = &container->items[i];
        if (item->type == UI_CONTAINER_ITEM_TYPE_BUTTON) {
            ui_button_t *button = (ui_button_t *)item->widget;
            if (UIButton_HandleMouseClick(button, x, y, down)) {
                handled = true;
                break;  // 只处理最上层组件的点击
            }
        }
    }
    return handled;
}

// 更新容器
void UIContainer_Update(ui_container_t *container, int msec) {
    if (!container || !container->config.visible) return;

    // 更新所有子组件
    for (int i = 0; i < container->item_count; i++) {
        ui_container_item_t *item = &container->items[i];
        if (item->type == UI_CONTAINER_ITEM_TYPE_BUTTON) {
            ui_button_t *button = (ui_button_t *)item->widget;
            UIButton_Update(button, msec);
        } else if (item->type == UI_CONTAINER_ITEM_TYPE_TEXT) {
            ui_text_t *text = (ui_text_t *)item->widget;
            UIText_Update(text, msec);
        }
    }
}

// 渲染容器
void UIContainer_Render(ui_container_t *container) {
    if (!container || !container->config.visible) return;

    const ui_container_config_t *cfg = &container->config;

    // 绘制背景
    canvas2d_set_fill_style(container->ctx, cfg->bg_color);
    canvas2d_fill_rect(container->ctx, cfg->x, cfg->y, cfg->width, cfg->height);

    // 绘制边框
    if (cfg->border_width > 0) {
        canvas2d_set_stroke_style(container->ctx, cfg->border_color);
        canvas2d_set_line_width(container->ctx, cfg->border_width);
        canvas2d_stroke_rect(container->ctx, cfg->x, cfg->y, cfg->width, cfg->height);
    }

    // 渲染所有子组件
    for (int i = 0; i < container->item_count; i++) {
        ui_container_item_t *item = &container->items[i];
        if (item->type == UI_CONTAINER_ITEM_TYPE_BUTTON) {
            ui_button_t *button = (ui_button_t *)item->widget;
            UIButton_Render(button);
        } else if (item->type == UI_CONTAINER_ITEM_TYPE_TEXT) {
            ui_text_t *text = (ui_text_t *)item->widget;
            UIText_Render(text);
        }
    }
}

// 清理容器
void UIContainer_Shutdown(ui_container_t *container) {
    if (!container) return;

    // 释放子组件数组（不释放子组件本身，由调用者管理）
    if (container->items) {
        free(container->items);
        container->items = NULL;
    }

    memset(container, 0, sizeof(ui_container_t));
}
