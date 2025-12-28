#include "ui_list.h"
#include "common/shared.h"
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// 初始化 UI 列表
int UIList_Init(ui_list_t *list, const ui_list_config_t *config, canvas2d_context_t *ctx) {
    if (!list || !config || !ctx) {
        return -1;
    }

    memset(list, 0, sizeof(ui_list_t));
    list->config = *config;
    list->ctx = ctx;
    list->item_count = 0;
    list->selected_index = 0;
    list->scroll_offset = 0;
    list->scroll_pos = 0.0f;
    list->is_scrolling = false;
    
    // 设置默认字体大小（如果未设置）
    if (list->config.font_size <= 0) {
        list->config.font_size = 14.0f;
    }

    // 计算可见项数量
    float total_item_height = config->item_height + config->item_spacing;
    list->visible_count = (int)(config->height / total_item_height);
    if (list->visible_count < 1) {
        list->visible_count = 1;
    }

    printf("UIList initialized: visible_count=%d, item_height=%.1f, spacing=%.1f\n",
           list->visible_count, config->item_height, config->item_spacing);

    return 0;
}

// 添加列表项
int UIList_AddItem(ui_list_t *list, const char *text, void *user_data) {
    if (!list || list->item_count >= UI_LIST_MAX_ITEMS) {
        return -1;
    }

    ui_list_item_t *item = &list->items[list->item_count];
    if (text) {
        strncpy(item->text, text, 255);
        item->text[255] = '\0';
    } else {
        item->text[0] = '\0';
    }
    item->user_data = user_data;

    return list->item_count++;
}

// 清空列表项
void UIList_ClearItems(ui_list_t *list) {
    if (!list) return;
    list->item_count = 0;
    list->selected_index = 0;
    list->scroll_offset = 0;
    list->scroll_pos = 0.0f;
}

// 设置选中项
void UIList_SetSelected(ui_list_t *list, int index) {
    if (!list) return;

    if (index < 0) {
        index = 0;
    } else if (index >= list->item_count && list->item_count > 0) {
        index = list->item_count - 1;
    }

    list->selected_index = index;

    // 自动滚动以确保选中项可见
    if (index < list->scroll_offset) {
        list->scroll_offset = index;
    } else if (index >= list->scroll_offset + list->visible_count) {
        list->scroll_offset = index - list->visible_count + 1;
    }
}

// 获取选中项
int UIList_GetSelected(ui_list_t *list) {
    if (!list) return -1;
    return list->selected_index;
}

// 获取选中项文本
const char* UIList_GetSelectedText(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    return list->items[list->selected_index].text;
}

// 获取选中项用户数据
void* UIList_GetSelectedUserData(ui_list_t *list) {
    if (!list || list->selected_index < 0 || list->selected_index >= list->item_count) {
        return NULL;
    }
    return list->items[list->selected_index].user_data;
}

// 获取列表项数量
int UIList_GetItemCount(ui_list_t *list) {
    if (!list) return 0;
    return list->item_count;
}

// 设置滚动位置
void UIList_SetScrollOffset(ui_list_t *list, int offset) {
    if (!list) return;

    if (offset < 0) {
        offset = 0;
    } else if (list->item_count > 0 && offset > list->item_count - list->visible_count) {
        offset = list->item_count - list->visible_count;
    }

    list->scroll_offset = offset;
}

// 获取滚动位置
int UIList_GetScrollOffset(ui_list_t *list) {
    if (!list) return 0;
    return list->scroll_offset;
}

// 处理键盘输入
bool UIList_HandleInput(ui_list_t *list, int key, bool down) {
    if (!list) return false;

    if (!down) return false;

    switch (key) {
        case SDLK_UP:
            if (list->selected_index > 0) {
                list->selected_index--;
                if (list->selected_index < list->scroll_offset) {
                    list->scroll_offset = list->selected_index;
                }
            }
            return true;

        case SDLK_DOWN:
            if (list->selected_index < list->item_count - 1) {
                list->selected_index++;
                if (list->selected_index >= list->scroll_offset + list->visible_count) {
                    list->scroll_offset = list->selected_index - list->visible_count + 1;
                }
            }
            return true;

        case SDLK_PAGEUP:
            UIList_SetSelected(list, list->selected_index - list->visible_count);
            return true;

        case SDLK_PAGEDOWN:
            UIList_SetSelected(list, list->selected_index + list->visible_count);
            return true;

        case SDLK_HOME:
            UIList_SetSelected(list, 0);
            return true;

        case SDLK_END:
            UIList_SetSelected(list, list->item_count - 1);
            return true;
    }

    return false;
}

// 处理鼠标点击
bool UIList_HandleMouseClick(ui_list_t *list, int x, int y) {
    if (!list) return false;

    const ui_list_config_t *cfg = &list->config;

    // 检查点击是否在列表区域内
    if (x < cfg->x || x >= cfg->x + cfg->width ||
        y < cfg->y || y >= cfg->y + cfg->height) {
        return false;
    }

    // 计算点击的是哪一项
    float total_item_height = cfg->item_height + cfg->item_spacing;
    int clicked_visible_index = (int)((y - cfg->y) / total_item_height);

    // 检查是否点击在有效范围内
    if (clicked_visible_index < 0 || clicked_visible_index >= list->visible_count) {
        return false;
    }

    // 计算实际项索引
    int clicked_index = list->scroll_offset + clicked_visible_index;

    // 检查索引是否有效
    if (clicked_index < 0 || clicked_index >= list->item_count) {
        return false;
    }

    // 选中点击的项
    UIList_SetSelected(list, clicked_index);
    return true;
}

// 处理鼠标滚轮
bool UIList_HandleMouseWheel(ui_list_t *list, int delta) {
    if (!list) return false;

    // delta > 0 表示向上滚动，delta < 0 表示向下滚动
    int scroll_amount = 0;

    if (delta > 0) {
        // 向上滚动
        scroll_amount = -1;
    } else if (delta < 0) {
        // 向下滚动
        scroll_amount = 1;
    }

    if (scroll_amount != 0) {
        int new_offset = list->scroll_offset + scroll_amount;
        int max_offset = list->item_count > list->visible_count
                         ? list->item_count - list->visible_count
                         : 0;

        if (new_offset >= 0 && new_offset <= max_offset) {
            list->scroll_offset = new_offset;
            list->is_scrolling = true;
            return true;
        }
    }

    return false;
}

// 更新列表
void UIList_Update(ui_list_t *list, int msec) {
    if (!list) return;

    // 平滑滚动动画
    if (list->is_scrolling) {
        // 这里可以添加滚动动画逻辑
        list->is_scrolling = false;
    }
}

// 渲染列表
void UIList_Render(ui_list_t *list) {
    if (!list) return;

    const ui_list_config_t *cfg = &list->config;
    float total_item_height = cfg->item_height + cfg->item_spacing;

    // 绘制背景
    canvas2d_set_fill_style(list->ctx, cfg->bg_color);
    canvas2d_fill_rect(list->ctx, cfg->x, cfg->y, cfg->width, cfg->height);

    
    // 先绘制滚动区域
    // 启用裁减区域，确保超出列表部分不渲染
    canvas2d_begin_clip(list->ctx, cfg->x, cfg->y, cfg->width, cfg->height);

    // 计算可见项：动态计算以确保部分可见的最后一个item也能渲染
    int start_index = list->scroll_offset;
    // 计算当前可以显示多少个完整item
    int max_visible = (int)(cfg->height / total_item_height);
    // 多加一个item，以确保最后一个部分可见的item也能被渲染
    int end_index = start_index + max_visible + 1;
    if (end_index > list->item_count) {
        end_index = list->item_count;
    }

    // 绘制列表项
    for (int i = start_index; i < end_index; i++) {
        int visible_index = i - start_index;
        float item_y = cfg->y + visible_index * total_item_height;
        bool selected = (i == list->selected_index);

        ui_list_item_t *item = &list->items[i];

        // 使用自定义绘制回调（如果提供）
        if (cfg->draw_callback) {
            cfg->draw_callback(i, item->text, cfg->user_data,
                              cfg->x, item_y, cfg->width, cfg->item_height, selected);
        } else {
            // 默认绘制
            // 背景
            COLOR32 bg = selected ? cfg->selected_bg_color : cfg->bg_color;
            canvas2d_set_fill_style(list->ctx, bg);
            canvas2d_fill_rect(list->ctx, cfg->x, item_y, cfg->width, cfg->item_height);

            // 文本（垂直居中）
            COLOR32 text_color = selected ? cfg->selected_text_color : cfg->text_color;
            canvas2d_set_fill_style(list->ctx, text_color);
            
            // 计算垂直居中位置
            float text_x = cfg->x + 10;
            float text_y = item_y + (cfg->item_height - cfg->font_size) / 2;
            canvas2d_fill_text(list->ctx, item->text, text_x, text_y);

            // 绘制项边框（仅默认绘制时）
            if (selected) {
                canvas2d_set_stroke_style(list->ctx, cfg->border_color);
                canvas2d_set_line_width(list->ctx, 2.0f);
                canvas2d_stroke_rect(list->ctx, cfg->x, item_y, cfg->width, cfg->item_height);
            }
        }
    }

    // 结束裁减
    canvas2d_end_clip(list->ctx);

    // 绘制边框
    canvas2d_set_stroke_style(list->ctx, cfg->border_color);
    canvas2d_set_line_width(list->ctx, 1.0f);
    canvas2d_stroke_rect(list->ctx, cfg->x, cfg->y, cfg->width, cfg->height);

    // 绘制滚动条
    if (cfg->show_scrollbar && list->item_count > list->visible_count) {
        UIList_DrawScrollbar(list);
    }
}

// 绘制滚动条
static void UIList_DrawScrollbar(ui_list_t *list) {
    const ui_list_config_t *cfg = &list->config;

    float scrollbar_width = 15.0f;
    float scrollbar_x = cfg->x + cfg->width - scrollbar_width;
    float scrollbar_height = cfg->height;

    // 滚动条滑块高度固定为单个列表项高度（item_height + item_spacing）
    float total_item_height = cfg->item_height + cfg->item_spacing;
    float thumb_height = total_item_height;

    // 计算可滚动的内容范围：从第一个item到最后一个item的总高度
    float total_content_height = list->item_count * total_item_height;
    // 可滚动范围 = 总内容高度 - 容器高度
    float scrollable_height = total_content_height - cfg->height;
    if (scrollable_height < 0) scrollable_height = 0;

    // 计算滚动条滑块位置
    float thumb_y;
    if (scrollable_height > 0) {
        // 当前滚动偏移的像素位置
        float scroll_offset_pixels = list->scroll_offset * total_item_height;
        // 滚动条可用移动范围 = 容器高度 - 滚动条滑块自身高度
        float scrollbar_range = scrollbar_height - thumb_height;
        // 滑块Y坐标 = 容器顶部 + (滚动偏移 / 可滚动范围) * 滚动条可用范围
        thumb_y = cfg->y + (scroll_offset_pixels / scrollable_height) * scrollbar_range;
        // 限制滑块在滚动条范围内
        if (thumb_y < cfg->y) thumb_y = cfg->y;
        if (thumb_y > cfg->y + scrollbar_range) thumb_y = cfg->y + scrollbar_range;
    } else {
        thumb_y = cfg->y;
    }

    // 绘制滚动条背景
    canvas2d_set_fill_style(list->ctx, (COLOR32){60, 60, 60, 255});
    canvas2d_fill_rect(list->ctx, scrollbar_x, cfg->y, scrollbar_width, scrollbar_height);

    // 绘制滚动条滑块
    canvas2d_set_fill_style(list->ctx, (COLOR32){150, 150, 150, 255});
    canvas2d_fill_rect(list->ctx, scrollbar_x, thumb_y, scrollbar_width, thumb_height);

    // 绘制滚动条边框
    canvas2d_set_stroke_style(list->ctx, (COLOR32){200, 200, 200, 255});
    canvas2d_set_line_width(list->ctx, 1.0f);
    canvas2d_stroke_rect(list->ctx, scrollbar_x, cfg->y, scrollbar_width, scrollbar_height);
}

// 清理列表
void UIList_Shutdown(ui_list_t *list) {
    if (!list) return;
    UIList_ClearItems(list);
}
