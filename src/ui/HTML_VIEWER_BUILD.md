# HTML Viewer 组件编译

## 添加到CMakeLists.txt

```cmake
# 添加HTML Viewer源文件
set(UI_COMPONENT_SOURCES
    src/ui/ui_component.c
    src/ui/ui_container.c
    src/ui/ui_button.c
    src/ui/ui_text.c
    src/ui/ui_list.c
    src/ui/ui_input.c
    src/ui/ui_html_viewer.c
)

add_library(ui_components STATIC ${UI_COMPONENT_SOURCES})

# 链接HTML库
target_link_libraries(ui_components
    html
    canvas2d
)
```

## 测试编译

```bash
# 编译测试程序
gcc -o test_html_viewer \
    src/ui/test_html_viewer.c \
    src/ui/ui_html_viewer.c \
    src/ui/ui_component.c \
    src/html/html.c \
    src/html/css.c \
    src/html/animation/anim_state.c \
    src/html/css/css_animation.c \
    src/html/css/css_gradient.c \
    src/html/renderer/render_gradient.c \
    src/html/renderer/render_utils.c \
    -I./src \
    -I./src/html \
    -lcanvas2d \
    -lpng \
    -lxml2 \
    -licuuc \
    -licuio \
    -lhubbub \
    -lcss
```

## 使用示例

参考 `ui_html_viewer_example.c` 了解在场景中的使用方式。
