# 统一事件系统重构文档

## 概述

本次重构将原有的 `ui_event_t` 和 `input_event_t` 统一为单一的事件系统，使用 `event_t` 类型。这样可以：
- 避免事件类型转换
- 简化事件处理流程
- 提高代码可维护性

## 新增文件

### `src/common/event.h`
统一的事件系统定义，包含：
- `event_type_t` 枚举：定义所有事件类型
- `event_t` 结构：统一的事件数据结构
- 事件操作函数：`Event_StopPropagation()`, `Event_PreventDefault()`
- `event_handler_t` 类型：事件处理器函数指针类型

### `src/common/event.c`
事件系统的基础实现。

## 修改的文件

### `src/common/scene.h`
- 移除了 `input_event_t` 和 `input_event_type_t` 的原始定义
- 添加了类型别名和宏定义以保持向后兼容：
  ```c
  typedef event_t input_event_t;
  typedef event_type_t input_event_type_t;
  #define INPUT_EVENT_KEY_DOWN  EVENT_KEY_DOWN
  // ... 其他宏定义
  ```

### `src/common/input_converter.h`
- 将 `input_event_t` 更新为 `event_t`

### `src/common/input_converter.c`
- 将 `input_event_t` 更新为 `event_t`
- 事件类型使用新的 `EVENT_*` 常量

### `src/ui/ui_component.h`
- 移除了 `ui_event_t` 的原始定义
- 添加了类型别名 `typedef event_t ui_event_t;`
- 事件处理器函数类型改为 `event_handler_t`
- 组件结构中的事件处理函数指针使用 `event_t` 类型

### `src/ui/ui_component.c`
- 包含了 `src/common/event.h`
- 所有 `ui_event_t` 引用更新为 `event_t`

## 事件类型对照表

| 原 INPUT_EVENT_* | 原 UI_EVENT_* | 新 EVENT_* |
|-----------------|--------------|------------|
| INPUT_EVENT_KEY_DOWN | EVENT_KEY_DOWN | EVENT_KEY_DOWN |
| INPUT_EVENT_KEY_UP | EVENT_KEY_UP | EVENT_KEY_UP |
| INPUT_EVENT_MOUSE_DOWN | EVENT_MOUSE_DOWN | EVENT_MOUSE_DOWN |
| INPUT_EVENT_MOUSE_UP | EVENT_MOUSE_UP | EVENT_MOUSE_UP |
| INPUT_EVENT_MOUSE_MOTION | EVENT_MOUSE_MOTION | EVENT_MOUSE_MOTION |
| INPUT_EVENT_MOUSE_WHEEL | EVENT_MOUSE_WHEEL | EVENT_MOUSE_WHEEL |
| INPUT_EVENT_CLICK | EVENT_CLICK | EVENT_CLICK |
| INPUT_EVENT_DOUBLE_CLICK | EVENT_DOUBLE_CLICK | EVENT_DOUBLE_CLICK |
| INPUT_EVENT_DRAG_START | EVENT_DRAG_START | EVENT_DRAG_START |
| INPUT_EVENT_DRAG | EVENT_DRAG | EVENT_DRAG |
| INPUT_EVENT_DRAG_END | EVENT_DRAG_END | EVENT_DRAG_END |
| INPUT_EVENT_MOUSE_ENTER | EVENT_MOUSE_ENTER | EVENT_MOUSE_ENTER |
| INPUT_EVENT_MOUSE_LEAVE | EVENT_MOUSE_LEAVE | EVENT_MOUSE_LEAVE |
| INPUT_EVENT_TEXT_INPUT | EVENT_TEXT_INPUT | EVENT_TEXT_INPUT |
| INPUT_EVENT_TEXT_EDITING | EVENT_TEXT_EDITING | EVENT_TEXT_EDITING |
| INPUT_EVENT_QUIT | EVENT_QUIT | EVENT_QUIT |
| - | EVENT_FOCUS | EVENT_FOCUS |
| - | EVENT_BLUR | EVENT_BLUR |
| - | EVENT_VALUE_CHANGED | EVENT_VALUE_CHANGED |
| - | EVENT_SCROLL | EVENT_SCROLL |
| - | EVENT_CHAR | EVENT_CHAR |
| - | EVENT_KEY_PRESS | EVENT_KEY_PRESS |
| - | EVENT_CONTEXT_MENU | EVENT_CONTEXT_MENU |
| - | EVENT_RESIZE | EVENT_RESIZE |

## 事件结构

```c
typedef struct event_t {
    // 基础信息
    event_type_t type;
    ui_component_t *target;
    ui_component_t *current_target;
    int timestamp;
    bool propagation_stopped;
    bool default_prevented;
    bool handled;
    void *user_data;

    // 事件数据（使用 union）
    union {
        struct { /* 鼠标事件 */ } mouse;
        struct { /* 滚轮事件 */ } wheel;
        struct { /* 键盘事件 */ } key;
        struct { /* 文本输入 */ } text;
        struct { /* 文本编辑 */ } editing;
        struct { /* 屏幕尺寸 */ } resize;
        struct { /* 滚动 */ } scroll;
    };

    // 向后兼容字段（扁平访问）
    float x;
    float y;
    int button;
    int key;
    unsigned int character;
    float scroll_delta;
} event_t;
```

## 向后兼容性

为了保持向后兼容，定义了类型别名：
```c
typedef event_t input_event_t;
typedef event_t ui_event_t;
typedef event_handler_t ui_event_handler_t;
```

因此，现有代码可以继续使用旧的类型名称，编译时不会出错。

## 使用示例

### 定义事件处理函数
```c
void on_button_click(ui_component_t *component, event_t *event) {
    if (event->type == EVENT_CLICK) {
        // 处理点击事件
        printf("Button clicked at (%.1f, %.1f)\n", event->mouse.x, event->mouse.y);
    }
}
```

### 事件处理器注册
```c
// 创建组件后设置事件处理函数
button->on_click = on_button_click;
```

### 或者使用事件处理器数组
```c
UIComponent_AddEventHandler(component, EVENT_CLICK, on_button_click, NULL);
```

## 注意事项

1. **头文件包含**：使用事件系统时，确保包含了 `src/common/event.h`（通常通过 `ui_component.h` 间接包含）

2. **事件类型**：使用 `EVENT_*` 前缀的枚举值，而不是 `INPUT_EVENT_*` 或 `UI_EVENT_*`

3. **事件字段访问**：
   - 新代码推荐使用 union 访问事件数据：`event->mouse.x`
   - 旧代码可以继续使用扁平访问：`event->x`

4. **编译**：需要确保 `src/common/event.c` 被编译并链接到项目中

## 迁移建议

对于现有代码，建议逐步迁移：
1. 首先将 `input_event_t` 和 `ui_event_t` 的使用更新为 `event_t`
2. 将 `INPUT_EVENT_*` 和 `UI_EVENT_*` 更新为 `EVENT_*`
3. 将事件处理函数指针类型更新为新的类型定义
4. （可选）使用 union 结构体访问事件数据以提高类型安全性
