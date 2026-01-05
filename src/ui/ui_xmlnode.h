#ifndef __UI_XMLNODE_H__
#define __UI_XMLNODE_H__

#include "../canvas2d/canvas2d.h"
#include "../common/shared.h"
#include "ui_component.h"
#include <libxml/tree.h>

// UI XML节点组件（继承自 ui_component_t）
typedef struct {
    ui_component_t base;              // 基础组件
} ui_xmlnode_t;

// 外部访问的虚函数表（用于HTML系统）
extern const ui_component_vtable_t g_xmlnode_vtable;

// 创建XML节点组件
ui_xmlnode_t* UIXMLNode_Create(float x, float y, float width, float height,
                                xmlNodePtr xml_node, canvas2d_context_t *ctx);

// 初始化XML节点组件
int UIXMLNode_Init(ui_xmlnode_t *xmlnode, xmlNodePtr node, canvas2d_context_t *ctx);

// 清理XML节点组件
void UIXMLNode_Shutdown(ui_xmlnode_t *xmlnode);

// 销毁XML节点组件
void UIXMLNode_Destroy(ui_xmlnode_t *xmlnode);

// 获取XML节点
xmlNodePtr UIXMLNode_GetXMLNode(const ui_xmlnode_t *xmlnode);

// 设置XML节点
void UIXMLNode_SetXMLNode(ui_xmlnode_t *xmlnode, xmlNodePtr node);

// 获取XML节点名称
const char* UIXMLNode_GetNodeName(const ui_xmlnode_t *xmlnode);

// 获取XML节点内容
char* UIXMLNode_GetNodeContent(const ui_xmlnode_t *xmlnode);

// 获取XML节点属性
char* UIXMLNode_GetNodeProperty(const ui_xmlnode_t *xmlnode, const char *prop_name);

// 设置组件位置
void UIXMLNode_SetPosition(ui_xmlnode_t *xmlnode, float x, float y);

// 设置组件大小
void UIXMLNode_SetSize(ui_xmlnode_t *xmlnode, float width, float height);

// 设置组件边界
void UIXMLNode_SetBounds(ui_xmlnode_t *xmlnode, float x, float y, float width, float height);

// 更新XML节点组件
void UIXMLNode_Update(ui_xmlnode_t *xmlnode, int msec);

// 渲染XML节点组件
void UIXMLNode_Render(ui_xmlnode_t *xmlnode);

#endif // __UI_XMLNODE_H__
