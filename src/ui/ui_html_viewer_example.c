#include "ui_html_viewer.h"
#include "../common/scene.h"
#include <stdio.h>

void on_element_clicked(ui_html_viewer_t *viewer, xmlNode *element, void *user_data) {
    if (element && element->name) {
        printf("Element clicked: %s\n", (char*)element->name);
    }
}

int scene_init(scene_t *scene, const scene_params_t *params) {
    ui_component_t *root = Scene_GetRootComponent(scene);
    if (!root) return -1;

    canvas2d_context_t *canvas_ctx = SceneManager_GetDefaultCanvasContext(scene->manager);

    ui_html_viewer_t *viewer = UIHTMLViewer_Create(50.0f, 50.0f, 800.0f, 600.0f, canvas_ctx);
    if (!viewer) return -1;

    if (UIHTMLViewer_LoadFromFile(viewer, "test.html") != 0) {
        UIHTMLViewer_Destroy(viewer);
        return -1;
    }

    UIHTMLViewer_SetElementClicked(viewer, on_element_clicked, NULL);
    UIHTMLViewer_SetZoom(viewer, 1.0f);

    UIContainer_AddChild((ui_container_t *)root, (ui_component_t *)viewer);
    scene->user_data = viewer;

    return 0;
}

void scene_shutdown(scene_t *scene) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)scene->user_data;
    if (viewer) {
        UIHTMLViewer_Destroy(viewer);
    }
}

scene_transition_t* scene_update(scene_t *scene, int msec) {
    ui_html_viewer_t *viewer = (ui_html_viewer_t *)scene->user_data;
    if (viewer) {
        UIHTMLViewer_Update(viewer, msec);
    }
    return NULL;
}

void scene_render(scene_t *scene) {
    Scene_RenderUI(scene);
}

scene_t html_viewer_scene = {
    .name = "HTML Viewer",
    .state = SCENE_STATE_UNINITIALIZED,
    .user_data = NULL,
    .root_component = NULL,
    .manager = NULL,
    .init = scene_init,
    .shutdown = scene_shutdown,
    .update = scene_update,
    .render = scene_render,
    .on_input = NULL,
    .pause = NULL,
    .resume = NULL
};
