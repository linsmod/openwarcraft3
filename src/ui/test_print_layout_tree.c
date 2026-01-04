/**
 * @file test_print_layout_tree.c
 * @brief 测试 HTML Viewer 打印布局树功能
 *
 * 使用方法：
 * 1. 编译: gcc -o test_print_layout_tree test_print_layout_tree.c ...（链接相关库）
 * 2. 运行: ./test_print_layout_tree
 * 3. 输出将显示 HTML 文件的布局树结构
 */

#include "ui_html_viewer.h"
#include "../common/scene.h"
#include "../canvas2d/canvas2d.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 测试打印布局树功能
 */
int test_print_layout_tree() {
    printf("========================================\n");
    printf("  HTML Viewer Layout Tree Print Test\n");
    printf("========================================\n\n");

    // 创建 canvas 上下文
    canvas2d_context_t *canvas_ctx = canvas2d_create_context(800, 600);
    if (!canvas_ctx) {
        fprintf(stderr, "Failed to create canvas context\n");
        return -1;
    }

    // 创建 HTML viewer
    ui_html_viewer_t *viewer = UIHTMLViewer_Create(0.0f, 0.0f, 800.0f, 600.0f, canvas_ctx);
    if (!viewer) {
        fprintf(stderr, "Failed to create HTML viewer\n");
        canvas2d_destroy_context(canvas_ctx);
        return -1;
    }

    // 测试 1: 打印未加载 HTML 的情况
    printf("\n--- Test 1: Print layout tree before loading HTML ---\n");
    UIHTMLViewer_PrintLayoutTree(viewer);

    // 加载 HTML 文件
    const char *html_file = "test.html";
    if (UIHTMLViewer_LoadFromFile(viewer, html_file) != 0) {
        fprintf(stderr, "Failed to load HTML file: %s\n", html_file);
        UIHTMLViewer_Destroy(viewer);
        canvas2d_destroy_context(canvas_ctx);
        return -1;
    }

    // 测试 2: 打印已加载 HTML 的布局树
    printf("\n--- Test 2: Print layout tree after loading HTML ---\n");
    UIHTMLViewer_PrintLayoutTree(viewer);

    // 测试 3: 更新 viewer 后再打印
    UIHTMLViewer_SetZoom(viewer, 1.5f);
    UIHTMLViewer_SetScroll(viewer, 10.0f, 20.0f);

    printf("\n--- Test 3: Print layout tree after setting zoom and scroll ---\n");
    UIHTMLViewer_PrintLayoutTree(viewer);

    // 测试 4: 打印 NULL viewer
    printf("\n--- Test 4: Print NULL viewer ---\n");
    UIHTMLViewer_PrintLayoutTree(NULL);

    // 清理
    UIHTMLViewer_Destroy(viewer);
    canvas2d_destroy_context(canvas_ctx);

    printf("\n========================================\n");
    printf("  All tests completed!\n");
    printf("========================================\n");

    return 0;
}

/**
 * @brief 简单示例：在场景中使用布局树打印
 */
int scene_with_layout_tree_print() {
    printf("\n========================================\n");
    printf("  Scene Example with Layout Tree Print\n");
    printf("========================================\n\n");

    // 创建场景管理器
    scene_manager_t *manager = SceneManager_Create();
    if (!manager) {
        fprintf(stderr, "Failed to create scene manager\n");
        return -1;
    }

    // 获取默认 canvas 上下文
    canvas2d_context_t *canvas_ctx = SceneManager_GetDefaultCanvasContext(manager);

    // 创建 HTML viewer 并添加到场景
    ui_html_viewer_t *viewer = UIHTMLViewer_Create(50.0f, 50.0f, 700.0f, 500.0f, canvas_ctx);
    if (!viewer) {
        SceneManager_Destroy(manager);
        return -1;
    }

    if (UIHTMLViewer_LoadFromFile(viewer, "test.html") != 0) {
        UIHTMLViewer_Destroy(viewer);
        SceneManager_Destroy(manager);
        return -1;
    }

    // 添加到场景根组件
    ui_component_t *root = Scene_GetRootComponent(SceneManager_GetCurrentScene(manager));
    if (root) {
        UIContainer_AddChild((ui_container_t *)root, (ui_component_t *)viewer);
    }

    // 打印 HTML viewer 的布局树
    printf("=== HTML Viewer Layout Tree ===\n");
    UIHTMLViewer_PrintLayoutTree(viewer);
    printf("===============================\n");

    // 打印整个 UI 组件树
    printf("\n=== UI Component Tree ===\n");
    UIComponent_PrintTree(root, 0);
    printf("========================\n");

    // 清理
    SceneManager_Destroy(manager);

    return 0;
}

/**
 * @brief 主函数
 */
int main(int argc, char *argv[]) {
    // 运行基本测试
    int result = test_print_layout_tree();
    if (result != 0) {
        return result;
    }

    // 运行场景示例
    result = scene_with_layout_tree_print();
    if (result != 0) {
        return result;
    }

    return 0;
}
