#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/html/html.c"

// 模拟的测试上下文
static context test_context;

// 模拟的HTML文档
static htmlDocPtr test_document;

int main() {
    printf("=== HTML Render Test Program ===\n");
    
    // 1. 创建测试HTML文档
    test_document = htmlNewDocNoDtD(NULL, NULL);
    if (!test_document) {
        printf("Failed to create test document\n");
        return 1;
    }
    
    // 创建HTML元素
    xmlNodePtr html_node = xmlNewDocNode(test_document, NULL, BAD_CAST "html", NULL);
    xmlDocSetRootElement(test_document, html_node);
    
    // 创建head元素
    xmlNodePtr head_node = xmlNewDocNode(test_document, NULL, BAD_CAST "head", NULL);
    xmlAddChild(html_node, head_node);
    
    // 创建title元素
    xmlNodePtr title_node = xmlNewDocNode(test_document, NULL, BAD_CAST "title", NULL);
    xmlNodeAddContent(title_node, BAD_CAST "Test HTML Page");
    xmlAddChild(head_node, title_node);
    
    // 创建body元素
    xmlNodePtr body_node = xmlNewDocNode(test_document, NULL, BAD_CAST "body", NULL);
    xmlAddChild(html_node, body_node);
    
    // 创建div元素
    xmlNodePtr div_node = xmlNewDocNode(test_document, NULL, BAD_CAST "div", NULL);
    xmlAddChild(body_node, div_node);
    
    // 创建h1元素
    xmlNodePtr h1_node = xmlNewDocNode(test_document, NULL, BAD_CAST "h1", NULL);
    xmlNodeAddContent(h1_node, BAD_CAST "Hello HTML Renderer!");
    xmlAddChild(div_node, h1_node);
    
    // 创建p元素
    xmlNodePtr p_node = xmlNewDocNode(test_document, NULL, BAD_CAST "p", NULL);
    xmlNodeAddContent(p_node, BAD_CAST "This is a test paragraph to demonstrate the HTML rendering capabilities.");
    xmlAddChild(div_node, p_node);
    
    // 创建input元素
    xmlNodePtr input_node = xmlNewDocNode(test_document, NULL, BAD_CAST "input", NULL);
    xmlNewProp(input_node, BAD_CAST "type", BAD_CAST "text");
    xmlNewProp(input_node, BAD_CAST "placeholder", BAD_CAST "Enter text here");
    xmlAddChild(div_node, input_node);
    
    // 初始化测试上下文
    memset(&test_context, 0, sizeof(test_context));
    test_context.document = test_document;
    
    // 初始化布局系统
    test_context.layout_ctx = malloc(sizeof(lay_context));
    lay_init_context(test_context.layout_ctx);
    lay_reset_context(test_context.layout_ctx);
    lay_reserve_items_capacity(test_context.layout_ctx, 100);
    
    // 设置文档布局
    userdata *doc_ud = malloc(sizeof(userdata));
    doc_ud->refcount = 0;
    doc_ud->layid = lay_item(test_context.layout_ctx);
    size2_t window = {800, 600};
    lay_set_size(test_context.layout_ctx, doc_ud->layid, (lay_vec2){window.width, window.height});
    test_context.document->_private = doc_ud;
    
    // 2. 初始化HTML渲染器
    if (html_render_init(&test_context) != 0) {
        printf("Failed to initialize HTML renderer\n");
        return 1;
    }
    
    printf("HTML Renderer initialized successfully\n");
    
    // 3. 设置渲染模式
    html_render_set_mode(HTML_RENDER_MODE_STATIC);
    
    // 4. 渲染几帧来测试
    printf("Rendering test frames...\n");
    for (int i = 0; i < 3; i++) {
        printf("Rendering frame %d...\n", i + 1);
        html_update_frame();
        printf("Frame %d rendered\n\n", i + 1);
    }
    
    // 5. 测试动画模式
    printf("Switching to animated mode...\n");
    html_render_set_mode(HTML_RENDER_MODE_ANIMATED);
    
    for (int i = 0; i < 2; i++) {
        printf("Rendering animated frame %d...\n", i + 1);
        html_update_frame();
        printf("Animated frame %d rendered\n\n", i + 1);
    }
    
    // 6. 清理
    printf("Cleaning up...\n");
    html_render_cleanup();
    
    // 清理XML文档
    xmlFreeDoc(test_document);
    lay_destroy_context(test_context.layout_ctx);
    
    printf("Test completed successfully!\n");
    return 0;
}