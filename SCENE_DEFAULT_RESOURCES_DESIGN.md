# Scene 默认资源系统 - 设计文档

## 设计目标

为所有场景提供共享的默认资源，减少重复代码，简化场景创建流程。

## 主要优势

1. **减少重复代码**：每个场景不再需要重复创建 canvas、layout context、event dispatcher 等
2. **统一资源管理**：所有默认资源由 SceneManager 统一创建和销毁
3. **向后兼容**：场景仍然可以创建自己的资源替换默认资源
4. **灵活性**：场景可以选择使用默认资源或创建自己的资源

## 修改内容

### 1. scene.h 修改

在 `scene_manager_t` 结构中添加默认资源字段：

```c
struct scene_manager_t {
    // ... 现有字段 ...
    
    // 默认资源系统
    void *default_canvas;              // 默认 canvas
    void *default_canvas_ctx;          // 默认 canvas 上下文
    void *default_lay_ctx;             // 默认 lay 布局上下文
    void *default_event_dispatcher;     // 默认事件分发器
    ui_component_t *default_root;     // 默认根容器
};
```

添加默认资源 API：

```c
// 创建场景管理器时指定 canvas 尺寸
scene_manager_t* SceneManager_Create(int width, int height);

// 获取默认资源
void* SceneManager_GetDefaultCanvas(scene_manager_t *mgr);
void* SceneManager_GetDefaultCanvasContext(scene_manager_t *mgr);
void* SceneManager_GetDefaultLayoutContext(scene_manager_t *mgr);
void* SceneManager_GetDefaultEventDispatcher(scene_manager_t *mgr);
ui_component_t* SceneManager_GetDefaultRoot(scene_manager_t *mgr);

// 使用默认资源初始化场景
void SceneManager_InitSceneWithDefaults(scene_manager_t *mgr, scene_t *scene);
```

### 2. scene.c 修改

- `SceneManager_Create` 创建默认资源
- `SceneManager_Destroy` 销毁默认资源
- 实现默认资源获取函数

## 使用示例

### 旧代码（需要自己创建所有资源）

```c
int MapSelect_Init(void) {
    // 创建 Canvas
    g_canvas = canvas2d_create(1024, 768);
    if (!g_canvas) return -1;
    g_ctx = canvas2d_get_context(g_canvas);
    
    // 初始化布局上下文
    lay_init_context(&g_lay_ctx);
    
    // 创建根容器
    g_root_container = UIContainer_Create(0, 0, 1024, 768, ...);
    
    // 初始化事件分发器
    UIEventDispatcher_Init(&g_event_dispatcher, g_root_container, g_ctx);
    
    // ... 创建UI组件 ...
}
```

### 新代码（使用默认资源）

```c
int MapSelectScene_Init(scene_t *scene, const scene_params_t *params) {
    scene_manager_t *mgr = scene->manager;
    
    // 获取默认资源
    g_canvas = SceneManager_GetDefaultCanvas(mgr);
    g_ctx = SceneManager_GetDefaultCanvasContext(mgr);
    g_lay_ctx = SceneManager_GetDefaultLayoutContext(mgr);
    g_event_dispatcher = SceneManager_GetDefaultEventDispatcher(mgr);
    
    // 获取默认根容器（可选，也可以创建自己的）
    g_root_container = SceneManager_GetDefaultRoot(mgr);
    
    // 直接创建UI组件，无需设置布局上下文等
    g_ui_list = UIList_Create(0, 0, 380, 500, 50, 14, g_ctx);
    UIContainer_AddChild((ui_container_t *)g_root_container, g_ui_list);
    
    // ... 其他UI组件 ...
}
```

### 简化版本（最简单的场景）

对于简单的场景，甚至不需要存储这些指针：

```c
int SimpleScene_Init(scene_t *scene, const scene_params_t *params) {
    scene_manager_t *mgr = scene->manager;
    canvas2d_context_t *ctx = SceneManager_GetDefaultCanvasContext(mgr);
    ui_component_t *root = SceneManager_GetDefaultRoot(mgr);
    
    // 直接使用默认资源创建UI
    ui_component_t *text = UIText_Create(0, 0, "Hello", gold, 20, 
        UI_TEXT_ALIGN_CENTER, UI_TEXT_VALIGN_MIDDLE, ctx);
    UIContainer_AddChild((ui_container_t *)root, text);
    
    return 0;
}
```

## 迁移指南

### 第一步：修改 main.c 中的 SceneManager 创建

```c
// 旧代码
scene_manager_t *scene_mgr = SceneManager_Create();

// 新代码
scene_manager_t *scene_mgr = SceneManager_Create(1024, 768);
```

### 第二步：简化场景初始化

移除场景中的：
- canvas 创建
- layout context 初始化
- 根容器创建
- 事件分发器初始化

使用 SceneManager 的默认资源替代。

### 第三步：更新场景关闭函数

移除场景中的资源清理代码（由 SceneManager 统一管理）。

## 注意事项

1. **生命周期管理**：默认资源由 SceneManager 管理生命周期，场景不要销毁它们
2. **线程安全**：默认资源是共享的，多线程场景需要额外同步
3. **自定义资源**：场景可以创建自己的资源替换默认资源
4. **向后兼容**：现有代码可以继续使用自己的资源，无需立即迁移

## TODO 项目

1. 在 scene.c 中实现默认资源的创建和销毁
2. 解决循环依赖问题（scene.h 和 canvas2d/ui_component）
3. 更新 map_select 场景使用默认资源
4. 更新其他场景（game_scene 等）使用默认资源
5. 编写单元测试验证默认资源系统
