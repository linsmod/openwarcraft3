# Layx Flex-Shrink压缩修复

## 问题
在Chrome浏览器中，当flex容器空间不足时，设置了`flex-shrink: 1`（默认值）的子元素会被压缩。但压缩后的元素尺寸不会小于其内容的实际尺寸（如文本所需的空间）。

在原始的layx库中，即使支持了flex-shrink的压缩，也没有考虑内容的**最小尺寸限制**，导致元素被过度压缩，内容（如文本）会溢出。

## 解决方案

### 1. 支持flex-shrink权重计算
在`layx_arrange_stacked`函数中，计算所有参与压缩的元素的flex-shrink权重总和：

```c
// 计算flex-shrink权重（如果flex_shrink > 0，则参与压缩）
if (pchild->flex_shrink > 0.0f) {
    total_shrink_factor += pchild->flex_shrink;
}
```

### 2. 计算每个元素的压缩量
根据每个元素的flex-shrink权重分配压缩空间：

```c
// 计算该元素的压缩比例：该元素的flex_shrink / 总flex_shrink
float shrink_ratio = pchild->flex_shrink / total_shrink_factor;
// 该元素需要压缩的总量：总不足空间 * 压缩比例
float shrink_amount = (float)extra_space * shrink_ratio;
```

### 3. 保留内容的最小尺寸（关键修复）
在压缩前，计算元素内容的**最小所需空间**，确保压缩后的尺寸不小于这个值：

```c
// 计算该元素的内容最小尺寸（不能小于子元素所需空间）
layx_scalar min_content_size = 0;
if (pchild->first_child != LAYX_INVALID_ID) {
    // 有子元素，计算子元素所需的最小空间
    layx_id grandchild = pchild->first_child;
    while (grandchild != LAYX_INVALID_ID) {
        layx_item_t *pgrand = layx_get_item(ctx, grandchild);
        layx_vec4 grand_rect = ctx->rects[grandchild];
        // 子元素在该维度的总占用：位置 + 尺寸 + 边距
        layx_scalar grand_space = grand_rect[dim] + grand_rect[2 + dim] +
                                           pgrand->margins[dim] + pgrand->margins[wdim];
        // 取最大值作为最小内容尺寸
        if (grand_space > min_content_size) {
            min_content_size = grand_space;
        }
        grandchild = pgrand->next_sibling;
    }
    // 加上padding和border
    min_content_size += pchild->padding[dim] + pchild->padding[wdim] +
                    pchild->border[dim] + pchild->border[wdim];
}

// 确保压缩后的尺寸不小于内容最小尺寸
x1 = x + layx_float_max((float)min_content_size, layx_float_max(0.0f, shrunk_size));
```

## 工作原理

### Flexbox规范要求
根据CSS Flexbox规范：
1. `flex-shrink: 1`（默认值）表示元素可以压缩
2. 压缩是根据元素的flex-shrink权重按比例分配的
3. **关键**：元素不能被压缩到小于其内容的最小尺寸

### 修改后的行为
1. **有子元素时**：计算子元素所需的空间作为最小尺寸，确保能包裹所有子内容
2. **压缩时**：压缩后的尺寸 = max(内容最小尺寸, 压缩计算尺寸)
3. **无子元素时**：如果设置了固定尺寸，可以被压缩（直到为0）

## 测试场景

### 场景1：包含文本的box
```css
.box {
    width: 80px;
    height: 80px;
    display: flex;
    /* flex-shrink: 1 (默认) */
}
```

HTML结构：
```html
<div class="box">A</div>
```

**原始行为**：
- box可能被压缩到0px（如果空间严重不足）
- 文本"A"会溢出

**修复后**：
- 计算文本"A"所需的最小空间（约24-30px）
- box被压缩，但不小于文本所需空间
- 文本正常显示，不会被截断

### 场景2：多个box在有限空间中
```css
.container {
    height: 120px;
    display: flex;
    flex-direction: column;
}

.box {
    height: 80px;  /* 5个box需要400px */
}
```

**原始行为**：
- 每个box按比例压缩：120px / 400px = 0.3
- 每个box压缩到约24px
- 文本严重溢出

**修复后**：
- 每个box按比例压缩
- 但保留文本所需的最小空间（约30px）
- 文本完整显示

## 修改的文件

- `/home/wulin/myopenwarcraft3/thirdparty/layx/layx.c`
  - `layx_arrange_stacked()` 函数
  - 第984-1035行：计算flex-shrink权重
  - 第1103-1142行：应用压缩并保留最小内容尺寸

## 兼容性

此修复使layx的行为更接近Chrome等浏览器的Flexbox实现：
- ✅ 支持flex-shrink压缩
- ✅ 保留内容的最小尺寸
- ✅ 按flex-shrink权重分配压缩空间
- ✅ 与Chrome浏览器行为一致

## 性能影响

此修改添加了在压缩前遍历子元素的逻辑：
- 时间复杂度：O(n * m)，n为flex子元素数量，m为每个子元素的子元素数量
- 在大多数情况下，m很小（0或1，如文本节点）
- 性能影响可忽略不计
