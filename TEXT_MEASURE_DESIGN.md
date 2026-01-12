# Layx 文本测量接口设计

## 概述

Layx 需要支持文本节点的尺寸测量，以便在 flex 布局压缩时保留最小内容尺寸，防止文本溢出。

## 核心设计

### 1. 文本测量回调类型

```c
/**
 * 文本测量回调函数
 * @param user_data    用户数据（通常指向 ui_component）
 * @param out_width    输出：文本宽度
 * @param out_height   输出：文本高度
 */
typedef void (*layx_measure_text_fn)(
    void *user_data,
    float *out_width,
    float *out_height
);
```

### 2. 扩展 layx_item_t

```c
typedef struct layx_item_t {
    layx_id id;
    layx_id parent;
    layx_id first_child;
    layx_id next_sibling;
    
    layx_vec2 size;
    layx_vec4 rect;
    layx_vec4 padding;
    layx_vec4 margin;
    layx_vec4 border;
    
    int width_set;
    int height_set;
    
    uint8_t type;           // LAYX_ITEM_TYPE_CONTAINER or LAYX_ITEM_TYPE_TEXT
    uint8_t flex_direction; // 0=row, 1=column
    
    // Flex 属性
    float flex_grow;
    float flex_shrink;
    
    // 新增：文本测量
    layx_measure_text_fn measure_text_fn;  // NULL 表示不是文本节点
    void *measure_text_user_data;          // 通常指向 ui_component
} layx_item_t;
```

### 3. API 函数

#### 设置文本测量回调
```c
/**
 * 为 item 设置文本测量回调
 * @param ctx           layx 上下文
 * @param item_id       item ID
 * @param fn            测量回调函数
 * @param user_data     用户数据（通常指向 ui_component）
 */
void layx_set_item_measure_callback(
    layx_context *ctx,
    layx_id item_id,
    layx_measure_text_fn fn,
    void *user_data
);
```

## 实现步骤

### Step 1: 扩展 layx.h
- 添加 `layx_measure_text_fn` 类型定义
- 扩展 `layx_item_t` 结构体（添加 `measure_text_fn` 和 `measure_text_user_data` 字段）
- 添加 `layx_set_item_measure_callback` 声明

### Step 2: 在 layx.c 中实现新函数

```c
void layx_set_item_measure_callback(
    layx_context *ctx,
    layx_id item_id,
    layx_measure_text_fn fn,
    void *user_data
) {
    layx_item_t *pitem = &ctx->items[item_id];
    pitem->measure_text_fn = fn;
    pitem->measure_text_user_data = user_data;
}
```

### Step 3: 在 layx_calc_size 中使用回调

```c
static layx_vec2 layx_calc_size(layx_context *ctx, layx_id id) {
    layx_item_t *pitem = &ctx->items[id];
    
    // 如果有测量回调（文本节点）
    if (pitem->measure_text_fn) {
        float text_width = 0, text_height = 0;
        pitem->measure_text_fn(pitem->measure_text_user_data, 
                               &text_width, &text_height);
        pitem->size.x = text_width;
        pitem->size.y = text_height;
        return pitem->size;
    }
    
    // ... 原有的容器尺寸计算逻辑
}
```

### Step 4: 在压缩时保留最小内容尺寸

```c
// 在 layx_arrange_stacked 中
for (layx_id i = child; i != LAYX_INVALID_ID; i = ctx->items[i].next_sibling) {
    layx_item_t *pchild = &ctx->items[i];
    
    // 计算内容最小尺寸（如果是文本节点）
    float min_content_size = 0;
    if (pchild->measure_text_fn) {
        float text_width, text_height;
        pchild->measure_text_fn(pchild->measure_text_user_data, 
                                &text_width, &text_height);
        min_content_size = (axis == 0) ? text_width : text_height;
    }
    
    // 加上 padding 和 border
    min_content_size += pchild->padding[axis] + pchild->padding[axis+2];
    min_content_size += pchild->border[axis] + pchild->border[axis+2];
    
    // 计算压缩量
    float shrink_amount = total_shrink > 0 
        ? deficit * (pchild->flex_shrink / total_shrink) 
        : 0;
    
    // 应用压缩但保留最小尺寸
    float new_size = pchild->size[axis] - shrink_amount;
    pchild->size[axis] = max(min_content_size, new_size);
}
```

## HTML 引擎集成示例

### 1. 实现测量回调

```c
static void html_measure_text_callback(
    void *user_data,
    float *out_width,
    float *out_height
) {
    // user_data 指向 ui_component
    ui_component_t *comp = (ui_component_t *)user_data;
    
    // 从 ui_component 获取文本信息
    // 假设 ui_component 中已有这些字段：
    // - text_content: 文本内容
    // - font_family: 字体家族
    // - font_size: 字体大小
    // - font_weight: 字体粗细
    
    const char *text = comp->text_content ? comp->text_content : "";
    const char *font = comp->font_family ? comp->font_family : "Arial";
    float font_size = comp->font_size > 0 ? comp->font_size : 12.0f;
    int font_weight = comp->font_weight;
    
    // 使用渲染引擎测量文本
    // 这里可以调用 FreeType、GDI、CoreText 等渲染 API
    measure_text_with_font(text, font, font_size, font_weight, 
                          out_width, out_height);
}
```

### 2. 创建文本节点时设置回调

```c
// 在创建文本节点时
layx_id text_item = layx_item(html->layout_ctx);
ui_component_t *comp = create_ui_component(...);

// 为 item 设置测量回调，user_data 指向 ui_component
layx_set_item_measure_callback(
    html->layout_ctx,
    text_item,
    html_measure_text_callback,
    comp  // 传递 ui_component 指针
);
```

## 优势

✅ **接口最简洁**：直接在 item 上设置回调，不需要额外的标记字段  
✅ **判断简单**：`pitem->measure_text_fn == NULL` 就能判断是否是文本节点  
✅ **完全解耦**：layx 不依赖任何渲染 API 和数据结构  
✅ **灵活可扩展**：每个 item 可以有自己的测量逻辑（支持不同字体引擎混合使用）  
✅ **准确可靠**：文本尺寸由实际渲染引擎测量  

## 测试用例

```c
// 测试 1: 设置文本测量回调
layx_id text1 = layx_item(ctx);
layx_set_item_measure_callback(ctx, text1, mock_measure_text, NULL);
// 期望：ctx->items[text1].measure_text_fn == mock_measure_text

// 测试 2: 非文本节点不设置回调
layx_id container = layx_item(ctx);
// 不设置回调
// 期望：ctx->items[container].measure_text_fn == NULL

// 测试 3: 计算尺寸时调用回调
layx_id text2 = layx_item(ctx);
layx_set_item_measure_callback(ctx, text2, mock_measure_text, test_data);
layx_calc_size(ctx, text2);
// 期望：mock_measure_text 被调用，text2 的 size 设置为测量结果

// 测试 4: Flex 压缩保留最小尺寸
layx_id box1 = layx_item(ctx);
layx_set_item_measure_callback(ctx, box1, mock_measure_text, test_data);
layx_set_size(ctx, box1, 80, 80);  // 设置固定尺寸 80x80
layx_set_flex_shrink(ctx, box1, 1); // 允许压缩
// 当容器空间不足时，box1 应该压缩到至少能包裹文本的尺寸
```

## 与之前方案的对比

| 方案 | 判断文本节点 | 设置复杂度 | 灵活性 |
|------|-------------|-----------|--------|
| **方案1（旧）** | `is_text_node == 1` + `ctx->measure_text_fn != NULL` | 需要分别设置标记和回调 | 所有 item 共享同一个回调 |
| **方案2（新）** | `measure_text_fn != NULL` | 只需要设置回调 | 每个 item 可以有不同的回调 |

**方案2更简洁直观！**
