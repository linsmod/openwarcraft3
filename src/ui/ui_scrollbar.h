#ifndef __UI_SCROLLBAR_H__
#define __UI_SCROLLBAR_H__

#include "ui_component.h"
#include "../common/shared.h"

// 滚动条方向
typedef enum {
    SCROLLBAR_ORIENTATION_VERTICAL = 0,  // 垂直滚动条
    SCROLLBAR_ORIENTATION_HORIZONTAL = 1  // 水平滚动条
} scrollbar_orientation_t;

// 滚动条结构
typedef struct {
    ui_component_t base;  // 基类：ui_component_t
    
    // 滚动条属性
    scrollbar_orientation_t orientation;  // 方向
    float scroll_value;                  // 当前滚动值 (0.0-1.0)
    float page_size;                    // 页面大小比例 (0.0-1.0)
    
    // 拖拽状态
    bool dragging;
    float drag_start_value;              // 拖拽开始时的滚动值
    float drag_start_pos;                // 拖拽开始时的鼠标位置
    
    // 样式配置
    int track_width;                    // 轨道宽度（垂直）或高度（水平）
    COLOR32 track_color;                 // 轨道颜色
    COLOR32 thumb_color;                 // 滑块颜色
    COLOR32 border_color;                // 边框颜色
    
    // 回调函数
    void (*on_scroll_changed)(ui_component_t *component, float value, void *user_data);
    void *user_data;
} ui_scrollbar_t;

// ==================== 创建和销毁 ====================

/**
 * @brief 创建滚动条组件
 * @param x X坐标
 * @param y Y坐标
 * @param width 宽度
 * @param height 高度
 * @param orientation 滚动条方向
 * @param ctx Canvas上下文
 * @return 滚动条组件指针
 */
ui_scrollbar_t* UIScrollbar_Create(float x, float y, float width, float height,
                                 scrollbar_orientation_t orientation,
                                 canvas2d_context_t *ctx);

/**
 * @brief 销毁滚动条组件
 * @param scrollbar 滚动条组件
 */
void UIScrollbar_Destroy(ui_scrollbar_t *scrollbar);

// ==================== 属性设置 ====================

/**
 * @brief 设置滚动值 (0.0-1.0)
 * @param scrollbar 滚动条组件
 * @param value 滚动值
 */
void UIScrollbar_SetValue(ui_scrollbar_t *scrollbar, float value);

/**
 * @brief 获取滚动值 (0.0-1.0)
 * @param scrollbar 滚动条组件
 * @return 滚动值
 */
float UIScrollbar_GetValue(const ui_scrollbar_t *scrollbar);

/**
 * @brief 设置页面大小比例 (0.0-1.0)
 * @param scrollbar 滚动条组件
 * @param page_size 页面大小比例
 */
void UIScrollbar_SetPageSize(ui_scrollbar_t *scrollbar, float page_size);

/**
 * @brief 设置滚动条样式
 * @param scrollbar 滚动条组件
 * @param track_color 轨道颜色
 * @param thumb_color 滑块颜色
 * @param border_color 边框颜色
 */
void UIScrollbar_SetColors(ui_scrollbar_t *scrollbar,
                        COLOR32 track_color,
                        COLOR32 thumb_color,
                        COLOR32 border_color);

/**
 * @brief 设置滚动变化回调
 * @param scrollbar 滚动条组件
 * @param callback 回调函数
 * @param user_data 用户数据
 */
void UIScrollbar_SetScrollCallback(ui_scrollbar_t *scrollbar,
                                 void (*callback)(ui_component_t *component, float value, void *user_data),
                                 void *user_data);

// ==================== 渲染和布局 ====================

/**
 * @brief 渲染滚动条（由系统自动调用）
 * @param component 滚动条组件
 */
void UIScrollbar_Render(ui_component_t *component);

// ==================== 事件处理 ====================

/**
 * @brief 滚动条鼠标按下处理
 * @param component 滚动条组件
 * @param event 事件
 */
void UIScrollbar_OnMouseDown(ui_component_t *component, event_t *event);

/**
 * @brief 滚动条鼠标移动处理
 * @param component 滚动条组件
 * @param event 事件
 */
void UIScrollbar_OnMouseMove(ui_component_t *component, event_t *event);

/**
 * @brief 滚动条鼠标抬起处理
 * @param component 滚动条组件
 * @param event 事件
 */
void UIScrollbar_OnMouseUp(ui_component_t *component, event_t *event);

#endif // __UI_SCROLLBAR_H__
