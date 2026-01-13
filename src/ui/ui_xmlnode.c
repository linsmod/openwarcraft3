#include "ui_xmlnode.h"
#include "common/event.h"
#include "common/shared.h"
#include "layx.h"
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
    .render = NULL,
    .render_background = NULL,
    .render_border = NULL,
    .set_position = NULL,
    .set_size = NULL,
    .set_bounds = NULL,
    .hit_test = NULL,
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
    .scroll_to = NULL,
    .scroll_by = NULL,
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
    UIComponent_SetSize((ui_component_t*)xmlnode, width, height);

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