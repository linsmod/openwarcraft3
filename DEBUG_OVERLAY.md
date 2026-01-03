# Debug Overlay Scene - 调试覆盖层

## 功能说明

调试覆盖层（Debug Overlay Scene）是一个特殊的场景，用于可视化显示当前鼠标命中的UI组件。

### 主要特性

1. **始终显示在顶层**：调试覆盖层不参与场景切换，始终渲染在所有其他场景之上
2. **不可交互**：不处理任何输入事件，仅用于显示调试信息
3. **可视化命中组件**：为当前鼠标命中的组件显示红色边框和类型信息

### 显示内容

- **红色边框**：围绕鼠标命中的组件绘制2像素宽的红色边框
- **组件信息**：在组件上方显示黄色文字，包含：
  - 组件类型名称（如Button, List, Container等）
  - 组件坐标 (x, y)
  - 组件尺寸 width x height

## 实现细节

### 1. scene_manager_t 扩展

在 `scene.h` 的 `scene_manager_t` 结构体中添加：

```c
scene_t *debug_overlay_scene;  // 调试覆盖层场景（始终显示在顶部）
```

### 2. HitTest 增强

在 `scene.c` 中修改 `SceneManager_HitTest` 函数，将命中的组件保存到场景管理器：

```c
ui_component_t* SceneManager_HitTest(scene_manager_t *mgr, float x, float y) {
    // ... 命中测试逻辑 ...

    ui_component_t *hit = component_hit_test(mgr->current_scene->root_component, x, y);

    // 保存命中的组件到mgr（用于调试覆盖层显示）
    mgr->mouse_target = hit;

    return hit;
}
```

### 3. 调试覆盖层场景创建

在 `SceneManager_Create` 中创建并初始化调试覆盖层：

```c
// 创建调试覆盖层场景
mgr->debug_overlay_scene = DebugOverlayScene_Create();
if (mgr->debug_overlay_scene) {
    mgr->debug_overlay_scene->manager = mgr;
    SCENE_INIT(mgr->debug_overlay_scene, NULL);
}
```

### 4. 渲染集成

在 `SceneManager_Render` 中，在渲染完所有场景后渲染调试覆盖层：

```c
// 渲染所有场景
for (int i = 0; i < mgr->stack_size; i++) {
    if (mgr->stack[i]->state == SCENE_STATE_ACTIVE ||
        mgr->stack[i]->state == SCENE_STATE_PAUSED) {
        Scene_RenderUI(mgr->stack[i]);
    }
}

// 最后渲染调试覆盖层（始终显示在顶部）
if (mgr->debug_overlay_scene) {
    DebugOverlayScene_Render(mgr->debug_overlay_scene);
}
```

### 5. 资源清理

在 `SceneManager_Destroy` 中销毁调试覆盖层：

```c
if (mgr->debug_overlay_scene) {
    SCENE_SHUTDOWN(mgr->debug_overlay_scene);
    free(mgr->debug_overlay_scene);
    mgr->debug_overlay_scene = NULL;
}
```

## 文件结构

```
src/
├── ui/
│   ├── debug_overlay_scene.h    # 调试覆盖层头文件
│   └── debug_overlay_scene.c    # 调试覆盖层实现
└── common/
    ├── scene.h                   # 场景管理器扩展（添加debug_overlay_scene字段）
    └── scene.c                   # 场景管理器实现（集成调试覆盖层）
```

## API 使用

### 创建场景管理器

调试覆盖层在创建场景管理器时自动初始化，无需额外配置：

```c
scene_manager_t *mgr = SceneManager_Create(width, height);
// debug_overlay_scene 已自动创建并初始化
```

### 渲染场景

渲染时自动包含调试覆盖层：

```c
SceneManager_Render(mgr);
// 会自动渲染调试覆盖层（如果存在鼠标命中的组件）
```

### 销毁场景管理器

销毁场景管理器时自动清理调试覆盖层：

```c
SceneManager_Destroy(mgr);
// debug_overlay_scene 已自动销毁
```

## 注意事项

1. **性能影响**：调试覆盖层会在每帧渲染，但由于逻辑简单，对性能影响很小
2. **仅在调试时启用**：生产环境可以移除或禁用此功能
3. **依赖鼠标状态**：只有当 `mgr->mouse_target` 不为NULL时才显示边框
4. **颜色配置**：当前使用硬编码的红色边框和黄色文字，可以根据需要调整

## 未来扩展

可以考虑的扩展功能：

1. **组件层级信息**：显示组件在UI树中的层级深度
2. **父组件信息**：显示父组件的名称
3. **事件处理信息**：显示组件是否处理了当前事件
4. **样式信息**：显示组件的颜色、字体等样式属性
5. **可配置显示**：通过配置文件或命令行参数控制显示哪些调试信息
