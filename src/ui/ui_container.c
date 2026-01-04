#include "ui_container.h"
#include "common/event.h"
#include "common/scene.h"
#include "html/layout.h"
#include "ui/ui_component.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// 子组件数组初始容量
#define CONTAINER_INITIAL_CAPACITY 8

// ==================== 虚函数实现 ====================

static void container_init(ui_component_t *component, canvas2d_context_t *ctx) {
    (void)component;
    (void)ctx;
}

static void container_shutdown(ui_component_t *component) {
    // 注意：不释放子组件，只释放数组
    if (component->children) {
        free(component->children);
        component->children = NULL;
    }
    component->child_count = 0;
    component->child_capacity = 0;
}

static void container_update(ui_component_t *component, int msec) {
    ui_container_t *container = (ui_container_t *)component;
    if (!container) return;

    // 更新所有子组件
    for (int i = 0; i < component->child_count; i++) {
        ui_component_t *child = component->children[i];
        if (child && child->vtable && child->vtable->update) {
            child->vtable->update(child, msec);
        }
    }
}

static void container_render(ui_component_t *component) {
    ui_container_t *container = (ui_container_t *)component;
    if (!container || !UIComponent_IsVisible(component)) return;

    // 绘制边框
    if (container->border_width > 0) {
        canvas2d_set_stroke_style(component->ctx, container->border_color);
        canvas2d_set_line_width(component->ctx, container->border_width);
        canvas2d_stroke_rect(component->ctx, component->x, component->y, component->width, component->height);
    }

    // 裁剪区域（如果需要）
    // if (component->flags & UI_FLAG_CLIPPING) {
        // canvas2d_save(component->ctx);
        // canvas2d_begin_path(component->ctx);
        // canvas2d_rect(component->ctx, component->x, component->y, component->width, component->height);
        // canvas2d_clip(component->ctx);
    // }

    // 渲染所有子组件
    // for (int i = 0; i < component->child_count; i++) {
    //     ui_component_t *child = component->children[i];
    //     if (child && child->vtable && child->vtable->render) {
    //         child->vtable->render(child);
    //     }
    // }

    // 恢复裁剪
    // if (component->flags & UI_FLAG_CLIPPING) {
        // canvas2d_restore(component->ctx);
    // }
}

static void container_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void container_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void container_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static ui_component_t *container_hit_test(ui_component_t *component, float x, float y) {
  // 1. 先判断点击是否在容器 bounds 内
  if (x >= component->x && x < component->x + component->width &&
      y >= component->y && y < component->y + component->height) {

    // 2. 如果有子组件，从后往前遍历（即“绘制顺序”的逆序：后绘制的在上层）
    if (component->children && component->child_count > 0) {
      for (int i = component->child_count - 1; i >= 0; i--) {
        ui_component_t *child = component->children[i];
        ui_component_t *hit_child = UIComponent_HitTest(child, x, y);
        if (hit_child) {
            return hit_child;  // 3. 一旦命中，立即返回（最上层命中的子）
        }
      }
    }

    // 4. 如果没有子组件命中，则容器自己响应（说明它是可交互的）
    return component;
  }
  return NULL; // 5. 点击不在容器区域内
}

// 容器特定的虚函数
// 容器特定的虚函数
static int container_add_child(ui_component_t *component, ui_component_t *child) {
    if (!component || !child) return -1;

    // 检查容量
    if (component->child_count >= component->child_capacity) {
        int new_capacity = component->child_capacity > 0 ? component->child_capacity * 2 : CONTAINER_INITIAL_CAPACITY;

        // 检查最大限制
        ui_container_t *container = (ui_container_t *)component;
        if (container->max_children > 0 && new_capacity > container->max_children) {
            new_capacity = container->max_children;
        }

        if (new_capacity <= component->child_capacity) {
            return -1; // 无法扩容
        }

        ui_component_t **new_children = realloc(component->children, sizeof(ui_component_t *) * new_capacity);
        if (!new_children) {
            return -1; // 内存分配失败
        }

        component->children = new_children;
        component->child_capacity = new_capacity;
    }

    // 添加子组件
    child->parent = component;
    component->children[component->child_count] = child;
    
    // 自动插入到布局系统
    if (component->lay_ctx) {
        // 确保子组件使用相同的布局上下文
        child->lay_ctx = component->lay_ctx;
        
        // 如果子组件没有layout item，创建一个
        if (child->lay_item_id == LAY_INVALID_ID) {
            child->lay_item_id = lay_item(child->lay_ctx);
        }
        
        // 如果父组件有layout item，插入到布局中
        if (component->lay_item_id != LAY_INVALID_ID && child->lay_item_id != LAY_INVALID_ID) {
            lay_insert(component->lay_ctx, component->lay_item_id, child->lay_item_id);
        }
    }
    
    return component->child_count++;
}

static bool container_remove_child(ui_component_t *component, ui_component_t *child) {
    if (!component || !child || !component->children) return false;

    // 查找子组件
    int index = -1;
    for (int i = 0; i < component->child_count; i++) {
        if (component->children[i] == child) {
            index = i;
            break;
        }
    }

    if (index == -1) return false;

    // 移动后面的组件
    for (int i = index; i < component->child_count - 1; i++) {
        component->children[i] = component->children[i + 1];
    }

    component->child_count--;
    child->parent = NULL;
    return true;
}

static int container_get_child_count(ui_component_t *component) {
    return component ? component->child_count : 0;
}

static ui_component_t* container_get_child(ui_component_t *component, int index) {
    if (!component || !component->children) return NULL;
    if (index < 0 || index >= component->child_count) return NULL;
    return component->children[index];
}

// container的print_tree实现：递归打印所有子组件
static void container_print_tree(const ui_component_t *component, int indent, const char* common) {
    if (!component) return;
    
    printf("%s\n", common);
    
    // 递归打印所有子组件
    for (int i = 0; i < component->child_count; i++) {
        UIComponent_PrintTree(component->children[i], indent + 1);
    }
}

// ==================== 虚函数表定义 ====================

static const ui_component_vtable_t g_container_vtable = {
    .init = container_init,
    .shutdown = container_shutdown,
    .update = container_update,
    .render = NULL,
    .set_position = container_set_position,
    .set_size = container_set_size,
    .set_bounds = container_set_bounds,
    .hit_test = container_hit_test,
    .on_mouse_enter = NULL,
    .on_mouse_leave = NULL,
    .on_mouse_down = NULL,
    .on_mouse_up = NULL,
    .on_click = NULL,
    .on_double_click = NULL,
    .on_mouse_move = NULL,
    .on_mouse_wheel = NULL,
    .on_context_menu = NULL,
    .on_drag_start = NULL,
    .on_drag = NULL,
    .on_drag_end = NULL,
    .on_key_down = NULL,
    .on_key_up = NULL,
    .on_text_input = NULL,
    .on_focus = NULL,
    .on_blur = NULL,
    .on_resize = NULL,
    .on_scroll = NULL,
    .add_child = container_add_child,
    .remove_child = container_remove_child,
    .get_child_count = container_get_child_count,
    .get_child = container_get_child,
    .get_custom_data = NULL,
    .set_custom_data = NULL,
    .print_tree = container_print_tree,
};

// ==================== 公共API实现 ====================

ui_container_t* UIContainer_Create(float x, float y, float width, float height,
                                  COLOR32 bg_color, COLOR32 border_color, canvas2d_context_t *ctx) {
    ui_container_t *container = malloc(sizeof(ui_container_t));
    if (!container) return NULL;

    if (UIContainer_Init(container, ctx) != 0) {
        free(container);
        return NULL;
    }

    container->base.x = x;
    container->base.y = y;
    UIComponent_SetMargin(&container->base, y, 0, 0, x);
    UIComponent_SetSize(&container->base, width, height);
    UIComponent_SetBgColor(&container->base, bg_color);
    container->border_color = border_color;
    container->border_width = 1.0f;
    container->max_children = 100;
    UIComponent_SetLayoutContain((ui_component_t*)container,LAY_COLUMN);

    // 启用裁剪
    // container->base.flags |= UI_FLAG_CLIPPING;

    return container;
}

int UIContainer_Init(ui_container_t *container, canvas2d_context_t *ctx) {
    if (!container || !ctx) {
        return -1;
    }

    // 使用新的组件系统初始化基础部分
    UIComponent_InitBase(&container->base, UI_COMPONENT_TYPE_CONTAINER, &g_container_vtable, ctx);

    // 使用基础组件的bg_color
    UIComponent_SetBgColor(&container->base, MAKE(COLOR32, 50, 50, 60, 255));
    container->border_color = MAKE(COLOR32, 100, 100, 100, 255);
    container->border_width = 1.0f;
    container->max_children = 100;

    // 启用裁剪
    container->base.flags |= UI_FLAG_CLIPPING;

    return 0;
}

void UIContainer_SetPosition(ui_container_t *container, float x, float y) {
    if (!container) return;
    if (container->base.vtable && container->base.vtable->set_position) {
        container->base.vtable->set_position(&container->base, x, y);
    }
}

void UIContainer_SetSize(ui_container_t *container, float width, float height) {
    if (!container) return;
    if (container->base.vtable && container->base.vtable->set_size) {
        container->base.vtable->set_size(&container->base, width, height);
    }
}

void UIContainer_SetBgColor(ui_container_t *container, COLOR32 color) {
    if (!container) return;
    UIComponent_SetBgColor(&container->base, color);
}

void UIContainer_SetBorderColor(ui_container_t *container, COLOR32 color) {
    if (!container) return;
    container->border_color = color;
}

int UIContainer_AddChild(ui_container_t *container, ui_component_t *child) {
    if (!container || !container->base.vtable) return -1;
    return container->base.vtable->add_child(&container->base, child);
}

bool UIContainer_RemoveChild(ui_container_t *container, ui_component_t *child) {
    if (!container || !container->base.vtable) return false;
    return container->base.vtable->remove_child(&container->base, child);
}

bool UIContainer_RemoveChildByIndex(ui_container_t *container, int index) {
    if (!container) return false;
    ui_component_t *child = UIContainer_GetChild(container, index);
    return child ? UIContainer_RemoveChild(container, child) : false;
}

void UIContainer_ClearChildren(ui_container_t *container) {
    if (!container) return;

    for (int i = 0; i < container->base.child_count; i++) {
        if (container->base.children[i]) {
            container->base.children[i]->parent = NULL;
        }
    }
    container->base.child_count = 0;
}

int UIContainer_GetChildCount(ui_container_t *container) {
    if (!container || !container->base.vtable) return 0;
    return container->base.vtable->get_child_count(&container->base);
}

ui_component_t* UIContainer_GetChild(ui_container_t *container, int index) {
    if (!container || !container->base.vtable) return NULL;
    return container->base.vtable->get_child(&container->base, index);
}

void UIContainer_Update(ui_container_t *container, int msec) {
    if (!container) return;
    if (container->base.vtable && container->base.vtable->update) {
        container->base.vtable->update(&container->base, msec);
    }
}

void UIContainer_Render(ui_container_t *container) {
    if (!container) return;
    if (container->base.vtable && container->base.vtable->render) {
        container->base.vtable->render(&container->base);
    }
}

void UIContainer_Shutdown(ui_container_t *container) {
    if (!container) return;

    if (container->base.vtable && container->base.vtable->shutdown) {
        container->base.vtable->shutdown(&container->base);
    }

    UIComponent_ShutdownBase(&container->base);
}

void UIContainer_Destroy(ui_container_t *container) {
    if (!container) return;
    UIContainer_Shutdown(container);
    free(container);
}
