# UI 组件事件系统迁移到统一 event_t

## 迁移概述

已成功将 UI 组件系统和场景管理系统迁移到使用统一的事件类型 `event_t`，替代原有的 `input_event_t` 和 `ui_event_t` 以及其子类型（如 `ui_mouse_event_t`, `ui_keyboard_event_t`）。

## 已完成的工作

### 1. 统一事件系统（已完成）
- ✅ 创建 `src/common/event.h` - 定义统一的 `event_t` 和 `event_type_t`
- ✅ 创建 `src/common/event.c` - 实现事件操作函数

### 2. 更新 vtable 事件处理函数签名（已完成）
- ✅ 修改 `ui_component.h` 中的 vtable，所有事件处理函数使用 `event_t`
- ✅ 修改 `ui_button.c` 中的所有事件处理函数使用 `event_t`

### 3. 更新场景管理器（已完成）
- ✅ 修改 `scene.h` 中移除 `input_event_t` 定义，添加类型别名
- ✅ 添加向后兼容宏（`#define INPUT_EVENT_* EVENT_*`）
- ✅ 更新 `SceneManager_BubbleEvent` 使用 `event_t`
- ✅ 更新 `SceneManager_ProcessEvent` 使用 `event_t`
- ✅ 更新 `SceneManager_OnInput` 使用 `event_t`
- ✅ 更新 `scene_manager_t` 结构使用 `event_t`

### 4. 更新输入转换器（已完成）
- ✅ 修改 `input_converter.h` 使用 `event_t`
- ✅ 修改 `input_converter.c` 使用 `event_t`

### 5. 更新主程序（已完成）
- ✅ 修改 `main.c` 使用 `event_t`

### 6. 更新 UI 组件基础（已完成）
- ✅ 修改 `ui_component.h` 使用 `event_t`
- ✅ 修改 `ui_component.c` 使用 `event_t`
- ✅ 添加类型别名 `typedef event_t ui_event_t;`

## 事件处理优先级

`SceneManager_BubbleEvent` 按以下优先级处理事件：

### 阶段1: Vtable 默认行为（最高优先级）
```c
if (current->vtable) {
    // 调用 vtable 中的事件处理函数
    handled = current->vtable->on_click(current, event);
}
```
**用途**：组件的默认行为，如按钮的状态切换（normal → hover → pressed）

### 阶段2: 用户自定义事件处理器
```c
if (!handled && event->type < UI_EVENT_MAX) {
    event_handler_t handler = current->event_handlers[event->type];
    if (handler) {
        handled = handler(current, event, user_data);
    }
}
```
**用途**：用户通过 `UIButton_AddOnClick(button, MyHandler, NULL)` 注册的处理器

### 阶段3: 组件直接事件处理函数指针（最低优先级）
```c
if (!handled) {
    if (current->on_click) {
        current->on_click(current, event);
    }
}
```
**用途**：动态设置的事件处理函数（较少使用）

## API 使用示例

### 定义组件事件处理函数（现在使用 event_t）
```c
static bool button_on_click(ui_component_t *component, event_t *event) {
    ui_button_t *button = (ui_button_t *)component;
    
    // 组件的默认行为（如状态管理）
    // ...
    
    return false;  // 返回 false 让事件继续传播到用户处理器
}
```

### 用户注册事件处理器
```c
void MyClickHandler(ui_component_t *component, event_t *event, void *user_data) {
    // 用户自定义逻辑
    printf("Button clicked!\n");
}

// 注册处理器
UIButton_AddOnClick(button, MyClickHandler, NULL);
```

### 访问事件数据（通过 union）
```c
void MyHandler(ui_component_t *component, event_t *event, void *user_data) {
    if (event->type == EVENT_CLICK) {
        // 访问鼠标坐标
        float x = event->mouse.x;
        float y = event->mouse.y;
        
        // 或使用扁平访问（向后兼容）
        float x2 = event->x;
        float y2 = event->y;
    }
    
    if (event->type == EVENT_KEY_DOWN) {
        // 访问键盘信息
        int key = event->key.key;
        bool down = event->key.down;
    }
}
```

## 向后兼容性

为了保持向后兼容，定义了以下类型别名和宏：

### 类型别名
```c
typedef event_t input_event_t;
typedef event_t ui_event_t;
typedef event_handler_t ui_event_handler_t;
```

### 宏定义
```c
#define INPUT_EVENT_KEY_DOWN       EVENT_KEY_DOWN
#define INPUT_EVENT_KEY_UP         EVENT_KEY_UP
#define INPUT_EVENT_MOUSE_DOWN     EVENT_MOUSE_DOWN
// ... 等等
```

这意味着现有代码可以继续使用旧的类型名称和常量，无需立即修改。

## 优势

1. **统一类型** - 场景和 UI 使用同一事件类型，无需转换
2. **类型安全** - 单一的 `event_t` 减少类型不匹配
3. **代码简洁** - `SceneManager_BubbleEvent` 更清晰，无重复代码
4. **易于扩展** - 添加新事件类型只需在一个地方定义
5. **向后兼容** - 旧代码可以继续使用

## 下一步工作

如果需要，可以逐步：
1. 迁移其他 UI 组件（label, text, list 等）使用 `event_t`
2. 逐步移除旧的 `ui_mouse_event_t`, `ui_keyboard_event_t` 等子类型定义
3. 更新文档和注释以反映新的统一事件系统
