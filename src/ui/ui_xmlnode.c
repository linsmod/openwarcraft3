#include "ui_xmlnode.h"
#include "common/shared.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==================== 虚函数实现 ====================

static void xmlnode_init(ui_component_t *component, canvas2d_context_t *ctx) {
    (void)component;
    (void)ctx;
}

static void xmlnode_shutdown(ui_component_t *component) {
    (void)component;
}

static void xmlnode_update(ui_component_t *component, int msec) {
    (void)component;
    (void)msec;
}

static void xmlnode_render(ui_component_t *component) {
    ui_xmlnode_t *xmlnode = (ui_xmlnode_t *)component;
    if (!xmlnode || !UIComponent_IsVisible(component)) return;

    // 默认渲染：绘制背景（如果有的话）
    COLOR32 bg_color = UIComponent_GetBgColor(component);
    if (bg_color.a > 0) {
        canvas2d_set_fill_style(component->ctx, bg_color);
        canvas2d_fill_rect(component->ctx, component->x, component->y,
                          component->width, component->height);
    }

    // 可以根据需要添加调试信息显示
    // 例如显示节点名称或内容
}

static void xmlnode_set_position(ui_component_t *component, float x, float y) {
    component->x = x;
    component->y = y;
}

static void xmlnode_set_size(ui_component_t *component, float width, float height) {
    component->width = width;
    component->height = height;
}

static void xmlnode_set_bounds(ui_component_t *component, float x, float y, float width, float height) {
    component->x = x;
    component->y = y;
    component->width = width;
    component->height = height;
}

static ui_component_t* xmlnode_hit_test(ui_component_t *component, float x, float y) {
    if (x >= component->x && x < component->x + component->width &&
        y >= component->y && y < component->y + component->height) {
        return component;
    }
    return NULL;
}

// xmlnode的print_tree实现：打印XML节点信息
static void xmlnode_print_tree(const ui_component_t *component, int indent, const char* common) {
    (void)indent;
    const ui_xmlnode_t *xmlnode = (const ui_xmlnode_t *)component;
    if (!xmlnode) return;

    printf("%s", common);

    // 打印节点名称
    // if (component->xml_node && component->xml_node->name) {
    //     printf(" node=\"%s\"", (const char*)component->xml_node->name);
    // } else {
    //     printf(" node=NULL");
    // }
    // printf("\n");
}

// ==================== 虚函数表定义 ====================

const ui_component_vtable_t g_xmlnode_vtable = {
    .init = xmlnode_init,
    .shutdown = xmlnode_shutdown,
    .update = xmlnode_update,
    .render = xmlnode_render,
    .render_background = NULL,
    .render_border = NULL,
    .set_position = xmlnode_set_position,
    .set_size = xmlnode_set_size,
    .set_bounds = xmlnode_set_bounds,
    .hit_test = xmlnode_hit_test,
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
    .add_child = NULL,
    .remove_child = NULL,
    .get_child_count = NULL,
    .get_child = NULL,
    .get_custom_data = NULL,
    .set_custom_data = NULL,
    .print_tree = xmlnode_print_tree,
};

// ==================== 公共API实现 ====================

ui_xmlnode_t* UIXMLNode_Create(float x, float y, float width, float height,
                                xmlNodePtr xml_node, canvas2d_context_t *ctx) {
    ui_xmlnode_t *xmlnode = malloc(sizeof(ui_xmlnode_t));
    if (!xmlnode) {
        return NULL;
    }

    if (UIXMLNode_Init(xmlnode, xml_node, ctx) != 0) {
        free(xmlnode);
        return NULL;
    }

    xmlnode->base.x = x;
    xmlnode->base.y = y;
    xmlnode->base.width = width;
    xmlnode->base.height = height;

    return xmlnode;
}

int UIXMLNode_Init(ui_xmlnode_t *xmlnode, xmlNodePtr node, canvas2d_context_t *ctx) {
    if (!xmlnode || !ctx) {
        return -1;
    }

    // 使用组件系统初始化基础部分
    UIComponent_InitBase(&xmlnode->base, UI_COMPONENT_TYPE_UNKNOWN, &g_xmlnode_vtable, ctx);

    // 设置XML节点
    xmlnode->base.xml_node = node;

    return 0;
}

void UIXMLNode_Shutdown(ui_xmlnode_t *xmlnode) {
    if (!xmlnode) return;

    // 调用虚函数shutdown
    if (xmlnode->base.vtable && xmlnode->base.vtable->shutdown) {
        xmlnode->base.vtable->shutdown(&xmlnode->base);
    }

    // 关闭基础组件
    UIComponent_ShutdownBase(&xmlnode->base);
}

void UIXMLNode_Destroy(ui_xmlnode_t *xmlnode) {
    if (!xmlnode) return;
    UIXMLNode_Shutdown(xmlnode);
    free(xmlnode);
}

xmlNodePtr UIXMLNode_GetXMLNode(const ui_xmlnode_t *xmlnode) {
    return xmlnode ? xmlnode->base.xml_node : NULL;
}

void UIXMLNode_SetXMLNode(ui_xmlnode_t *xmlnode, xmlNodePtr node) {
    if (xmlnode) {
        xmlnode->base.xml_node = node;
    }
}


char* UIXMLNode_GetNodeContent(const ui_xmlnode_t *xmlnode) {
    if (!xmlnode || !xmlnode->base.xml_node) {
        return NULL;
    }
    return (char*)xmlNodeGetContent(xmlnode->base.xml_node);
}

char* UIXMLNode_GetNodeProperty(const ui_xmlnode_t *xmlnode, const char *prop_name) {
    if (!xmlnode || !xmlnode->base.xml_node || !prop_name) {
        return NULL;
    }
    return (char*)xmlGetProp(xmlnode->base.xml_node, (const xmlChar*)prop_name);
}

void UIXMLNode_SetPosition(ui_xmlnode_t *xmlnode, float x, float y) {
    if (!xmlnode) return;
    if (xmlnode->base.vtable && xmlnode->base.vtable->set_position) {
        xmlnode->base.vtable->set_position(&xmlnode->base, x, y);
    }
}

void UIXMLNode_SetSize(ui_xmlnode_t *xmlnode, float width, float height) {
    if (!xmlnode) return;
    if (xmlnode->base.vtable && xmlnode->base.vtable->set_size) {
        xmlnode->base.vtable->set_size(&xmlnode->base, width, height);
    }
}

void UIXMLNode_SetBounds(ui_xmlnode_t *xmlnode, float x, float y, float width, float height) {
    if (!xmlnode) return;
    if (xmlnode->base.vtable && xmlnode->base.vtable->set_bounds) {
        xmlnode->base.vtable->set_bounds(&xmlnode->base, x, y, width, height);
    }
}

void UIXMLNode_Update(ui_xmlnode_t *xmlnode, int msec) {
    if (!xmlnode) return;
    if (xmlnode->base.vtable && xmlnode->base.vtable->update) {
        xmlnode->base.vtable->update(&xmlnode->base, msec);
    }
}

void UIXMLNode_Render(ui_xmlnode_t *xmlnode) {
    if (!xmlnode) return;
    if (xmlnode->base.vtable && xmlnode->base.vtable->render) {
        xmlnode->base.vtable->render(&xmlnode->base);
    }
}
