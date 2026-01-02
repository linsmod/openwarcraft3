#include "ui_html_viewer.h"
#include "ui_container.h"
#include "../canvas2d/canvas2d.h"
#include <stdio.h>
#include <stdlib.h>

void on_click(ui_html_viewer_t *viewer, xmlNode *element, void *data) {
    printf("Clicked: %s\n", element ? (char*)element->name : "null");
}

int main() {
    int width = 800;
    int height = 600;

    canvas2d_t *canvas = canvas2d_create(width, height);
    if (!canvas) {
        printf("Failed to create canvas\n");
        return 1;
    }

    canvas2d_context_t *ctx = canvas2d_get_context(canvas);

    ui_html_viewer_t *viewer = UIHTMLViewer_Create(50, 50, 700, 500, ctx);
    if (!viewer) {
        printf("Failed to create viewer\n");
        canvas2d_destroy(canvas);
        return 1;
    }

    if (UIHTMLViewer_LoadFromFile(viewer, "test.html") != 0) {
        printf("Failed to load HTML\n");
        UIHTMLViewer_Destroy(viewer);
        canvas2d_destroy(canvas);
        return 1;
    }

    UIHTMLViewer_SetElementClicked(viewer, on_click, NULL);

    printf("Running...\n");

    for (int i = 0; i < 100; i++) {
        UIHTMLViewer_Update(viewer, 16);

        canvas2d_set_fill_style(ctx, (COLOR32){30, 30, 30, 255});
        canvas2d_fill_rect(ctx, 0, 0, width, height);

        UIHTMLViewer_Render(viewer);
    }

    printf("Done\n");

    UIHTMLViewer_Destroy(viewer);
    canvas2d_destroy(canvas);

    return 0;
}
