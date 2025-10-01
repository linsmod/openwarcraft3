// canvas2d.c
#include "canvas2d.h"
#include "client/renderer.h"
#include "cmath3/types/rect.h"
#include "r_local.h"
#include "../common/shared.h"
#include <StormPort.h>
#include <math.h>
#include <unistd.h>

// 使用引擎的内存管理函数
#define MemAlloc(size) malloc(size)
#define MemFree(ptr) free(ptr)

RECT* NormRect(RECT* rect){
    rect->x = NORM(rect->x);
    rect->y = NORM(rect->y);
    rect->w = NORM(rect->w);
    rect->h = NORM(rect->h);
    return rect;
}
// 修改 DRAWIMAGE 宏以接受变换矩阵和混合模式
#define DRAWIMAGE(t, s, u, c, matrix) \
    do { \
        DRAWIMAGE drawImg = { \
            .texture = t, \
            .screen = *NormRect(s), \
            .uv = u ? *NormRect(u) : (RECT){0,0,1,1}, \
            .color = c, \
            .rotate = false, \
            .shader = SHADER_UI, \
            .alphamode = BLEND_MODE_BLEND, \
            .model_matrix = matrix \
        }; \
        R_DrawImageEx(&drawImg); \
    } while (0)

// Error handling
static char canvas2d_error_string[256] = {0};

const char* canvas2d_get_error_string(void) {
    return canvas2d_error_string;
}

void canvas2d_log_error(const char *message) {
    snprintf(canvas2d_error_string, sizeof(canvas2d_error_string), "%s", message);
    printf("Canvas2D Error: %s\n", message);
}

// Canvas creation and destruction
canvas2d_t* canvas2d_create(int width, int height) {
    canvas2d_t *canvas = (canvas2d_t*)MemAlloc(sizeof(canvas2d_t));
    if (!canvas) {
        canvas2d_log_error("Failed to allocate memory for canvas");
        return NULL;
    }

    canvas->width = width;
    canvas->height = height;
    canvas->initialized = true;
    
    // Create context
    canvas->context = (canvas2d_context_t*)MemAlloc(sizeof(canvas2d_context_t));
    if (!canvas->context) {
        canvas2d_log_error("Failed to allocate memory for context");
        MemFree(canvas);
        return NULL;
    }

    // Initialize context
    canvas->context->canvas = canvas;
    canvas->context->state.strokeStyle = COLOR32_BLACK;
    canvas->context->state.fillStyle = COLOR32_BLACK;
    canvas->context->state.lineWidth = 1;
    canvas->context->state.lineCap = CANVAS2D_BUTT;
    canvas->context->state.lineJoin = CANVAS2D_MITER_JOIN;
    canvas->context->state.miterLimit = 10;
    canvas->context->state.globalCompositeOperation = CANVAS2D_SOURCE_OVER;
    
    // Initialize transform matrix to identity
    Matrix4_identity(&canvas->context->state.transformMatrix);

    // Path
    canvas->context->state.pathPoints = NULL;           // 初始为空指针
    canvas->context->state.pathPointsCount = 0;         // 没有点
    canvas->context->state.pathPointsCapacity = 0;      // 容量为0
    canvas->context->state.pathOpen = false;            // 路径未开启
    
    // Initialize state stack
    canvas->context->stateStack = (canvas2d_state_t*)MemAlloc(sizeof(canvas2d_state_t) * 10);
    canvas->context->stateStackSize = 0;
    canvas->context->stateStackCapacity = 10;

    return canvas;
}

void canvas2d_destroy(canvas2d_t *canvas) {
    if (!canvas) return;

    if (canvas->context) {
        if (canvas->context->state.pathPoints) {
            MemFree(canvas->context->state.pathPoints);
        }

        if (canvas->context->stateStack) {
            MemFree(canvas->context->stateStack);
        }
        MemFree(canvas->context);
    }

    MemFree(canvas);
}

canvas2d_context_t* canvas2d_get_context(canvas2d_t *canvas) {
    if (!canvas || !canvas->initialized) {
        canvas2d_log_error("Canvas not initialized");
        return NULL;
    }
    return canvas->context;
}

// Context operations
void canvas2d_clear_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    // 使用半透明黑色来创建覆盖效果，而不是完全透明
    COLOR32 clearColor = {0, 0, 0, 64}; // 25% 透明度的黑色
    RECT screen = {x, y, width, height};
    RECT uv = {0, 0, 1, 1};
    
    DRAWIMAGE(tr.texture[TEX_WHITE], &screen, &uv, clearColor, &ctx->state.transformMatrix);
}

void canvas2d_fill_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    RECT screen = {x,y,width,height};
    RECT uv = {0, 0, 1, 1};
    
    DRAWIMAGE(tr.texture[TEX_WHITE], &screen, &uv, ctx->state.fillStyle, &ctx->state.transformMatrix);
}

void canvas2d_stroke_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    float lineWidth = ctx->state.lineWidth;
    
    // 绘制四条边
    RECT top = {x, y, width, lineWidth};
    RECT bottom = {x, y + height - lineWidth, width, lineWidth};
    RECT left = {x, y, lineWidth, height};
    RECT right = {x + width - lineWidth, y, lineWidth, height};
    
    RECT uv = {0, 0, 1, 1};
    
    DRAWIMAGE(tr.texture[TEX_WHITE], &top, &uv, ctx->state.strokeStyle, &ctx->state.transformMatrix);
    DRAWIMAGE(tr.texture[TEX_WHITE], &bottom, &uv, ctx->state.strokeStyle, &ctx->state.transformMatrix);
    DRAWIMAGE(tr.texture[TEX_WHITE], &left, &uv, ctx->state.strokeStyle, &ctx->state.transformMatrix);
    DRAWIMAGE(tr.texture[TEX_WHITE], &right, &uv, ctx->state.strokeStyle, &ctx->state.transformMatrix);
}

// 简化的圆形绘制（使用纹理近似）
void canvas2d_fill_circle(canvas2d_context_t *ctx, float x, float y, float radius) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    RECT screen = {x - radius, y - radius, radius * 2, radius * 2};
    RECT uv = {0, 0, 1, 1};
    
    // 使用圆形纹理（如果有的话），否则使用白色纹理
    LPCTEXTURE circleTex = tr.texture[TEX_WHITE]; // 可以替换为专门的圆形纹理
    DRAWIMAGE(circleTex, &screen, &uv, ctx->state.fillStyle, &ctx->state.transformMatrix);
}

void canvas2d_stroke_circle(canvas2d_context_t *ctx, float x, float y, float radius) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    // 简化的圆形描边：绘制多个小矩形来近似
    const int segments = 32;
    float angleStep = (2.0f * M_PI) / segments;
    float lineWidth = ctx->state.lineWidth;

    for (int i = 0; i < segments; i++) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        float x1 = x + cos(angle1) * radius;
        float y1 = y + sin(angle1) * radius;
        float x2 = x + cos(angle2) * radius;
        float y2 = y + sin(angle2) * radius;

        // 计算线段的方向和长度
        float dx = x2 - x1;
        float dy = y2 - y1;
        float length = sqrt(dx * dx + dy * dy);
        
        if (length > 0) {
            dx /= length;
            dy /= length;
        }

        // 创建线段矩形
        RECT lineRect = {
            x1 - (dy * lineWidth) / 2,
            y1 + (dx * lineWidth) / 2,
            length,
            lineWidth
        };
        
        RECT uv = {0, 0, 1, 1};
        DRAWIMAGE(tr.texture[TEX_WHITE], &lineRect, &uv, ctx->state.strokeStyle, &ctx->state.transformMatrix);
    }
}

// 文本绘制
void canvas2d_fill_text(canvas2d_context_t *ctx, const char *text, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !text) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    // DRAWTEXT drawtext =  get_drawtext_html(NULL, ctx->state.fillStyle,1,text,
    //     FONT_JUSTIFYLEFT,
    //     FONT_JUSTIFYTOP
    // );
    // drawtext.rect = (RECT){NORM(x), NORM(y), 0.2, 0.2};
    // drawtext.model_matrix = &ctx->state.transformMatrix;
    // R_DrawUtf8TextEx(&drawtext);

    // 使用UTF8版本的渲染函数，支持中文显示
    RECT box = (RECT){NORM(x), NORM(y), 1, 1};
    R_DrawUtf8Text2(text, box, ctx->state.fillStyle,NULL, &ctx->state.transformMatrix);
}

void canvas2d_stroke_text(canvas2d_context_t *ctx, const char *text, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !text) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    // 简化的描边文本：在多个偏移位置绘制文本
    float offset = ctx->state.lineWidth;
    COLOR32 strokeColor = ctx->state.strokeStyle;
    
    for (float dx = -offset; dx <= offset; dx += offset/2) {
        for (float dy = -offset; dy <= offset; dy += offset/2) {
            if (dx == 0 && dy == 0) continue;
            RECT rect = MAKE(RECT,NORM(x + dx),NORM(y + dy),1-NORM(x + dx),1-NORM(x + dx));
            R_DrawUtf8Text2(text, rect, strokeColor,NULL,&ctx->state.transformMatrix);
        }
    }
    RECT rect = MAKE(RECT,NORM(x) ,NORM(y) ,1-NORM(x),1-NORM(y));
    // 绘制主体文本
    R_DrawUtf8Text2(text, rect,
    ctx->state.fillStyle,NULL, &ctx->state.transformMatrix);
}

// 图像绘制
void canvas2d_draw_image(canvas2d_context_t *ctx, LPCTEXTURE texture, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !texture) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    RECT screen = {x, y, (float)texture->width, (float)texture->height};
    DRAWIMAGE(texture, &screen, NULL, COLOR32_WHITE, &ctx->state.transformMatrix);
}

void canvas2d_draw_image_scaled(canvas2d_context_t *ctx, LPCTEXTURE texture, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !texture) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    RECT screen = {x, y, width, height};
    DRAWIMAGE(texture, &screen, NULL, COLOR32_WHITE, &ctx->state.transformMatrix);
}

// 变换操作
void canvas2d_translate(canvas2d_context_t *ctx, float x, float y) {
    if (!ctx) return;
    
    MATRIX4 translation;
    Matrix4_identity(&translation);
    Matrix4_translate(&translation, &(VECTOR3){NORM(x), NORM(y), 0});
    Matrix4_multiply(&ctx->state.transformMatrix, &translation, &ctx->state.transformMatrix);
}

void canvas2d_rotate(canvas2d_context_t *ctx, float angle) {
    if (!ctx) return;
    
    MATRIX4 rotation;
    Matrix4_identity(&rotation);
    Matrix4_rotate(&rotation, &(VECTOR3){0, 0, angle * 180 / M_PI}, ROTATE_XYZ);
    Matrix4_multiply(&ctx->state.transformMatrix, &rotation, &ctx->state.transformMatrix);
}

void canvas2d_scale(canvas2d_context_t *ctx, float x, float y) {
    if (!ctx) return;
    
    MATRIX4 scaling;
    Matrix4_identity(&scaling);
    Matrix4_scale(&scaling, &(VECTOR3){NORM(x), NORM(y), 1});
    Matrix4_multiply(&ctx->state.transformMatrix, &scaling, &ctx->state.transformMatrix);
}

void canvas2d_reset_transform(canvas2d_context_t *ctx) {
    if (!ctx) return;
    Matrix4_identity(&ctx->state.transformMatrix);
}

// 状态管理
void canvas2d_save(canvas2d_context_t *ctx) {
    if (!ctx) return;

    if (ctx->stateStackSize >= ctx->stateStackCapacity) {
        ctx->stateStackCapacity *= 2;
        ctx->stateStack = realloc(ctx->stateStack, ctx->stateStackCapacity * sizeof(canvas2d_state_t));
    }

    memcpy(&ctx->stateStack[ctx->stateStackSize], &ctx->state, sizeof(canvas2d_state_t));

    // Path: deep clone
    if (ctx->state.pathOpen && ctx->state.pathPointsCount > 0) {
        // 分配新的路径点数组
        ctx->stateStack[ctx->stateStackSize].pathPoints = (VECTOR2*)MemAlloc(sizeof(VECTOR2) * ctx->state.pathPointsCapacity);
        if (ctx->stateStack[ctx->stateStackSize].pathPoints) {
            memcpy(ctx->stateStack[ctx->stateStackSize].pathPoints, 
                   ctx->state.pathPoints, 
                   sizeof(VECTOR2) * ctx->state.pathPointsCount);
        }
    } else {
        // 路径未开启或为空，设置为NULL
        ctx->stateStack[ctx->stateStackSize].pathPoints = NULL;
        ctx->stateStack[ctx->stateStackSize].pathPointsCount = 0;
        ctx->stateStack[ctx->stateStackSize].pathPointsCapacity = 0;
    }

    ctx->stateStackSize++;
}

void canvas2d_restore(canvas2d_context_t *ctx) {
    if (!ctx || ctx->stateStackSize <= 0) return;

    ctx->stateStackSize--;
    memcpy(&ctx->state, &ctx->stateStack[ctx->stateStackSize], sizeof(canvas2d_state_t));

    // path
    if (ctx->stateStack[ctx->stateStackSize].pathPoints) {
        // 分配新内存并复制路径数据
        ctx->state.pathPoints = (VECTOR2*)MemAlloc(sizeof(VECTOR2) * ctx->state.pathPointsCapacity);
        if (ctx->state.pathPoints) {
            memcpy(ctx->state.pathPoints, 
                   ctx->stateStack[ctx->stateStackSize].pathPoints, 
                   sizeof(VECTOR2) * ctx->state.pathPointsCount);
        }
        
        // 释放栈中的路径数据
        MemFree(ctx->stateStack[ctx->stateStackSize].pathPoints);
        ctx->stateStack[ctx->stateStackSize].pathPoints = NULL;
    }
}
void canvas2d_reset_path(canvas2d_context_t *ctx) {
    if (!ctx) return;
    
    if (ctx->state.pathPoints) {
        MemFree(ctx->state.pathPoints);
        ctx->state.pathPoints = NULL;
    }
    
    ctx->state.pathPointsCount = 0;
    ctx->state.pathPointsCapacity = 0;
    ctx->state.pathOpen = false;
}
void canvas2d_transform(canvas2d_context_t *ctx, float a, float b, float c, float d, float e, float f) {
    if (!ctx) return;
    
    MATRIX4 customTransform = {
        .v = {
            a, b, 0, 0,
            c, d, 0, 0,
            0, 0, 1, 0,
            e, f, 0, 1
        }
    };
    
    Matrix4_multiply(&ctx->state.transformMatrix, &customTransform, &ctx->state.transformMatrix);
}

void canvas2d_set_transform(canvas2d_context_t *ctx, float a, float b, float c, float d, float e, float f) {
    if (!ctx) return;
    
    ctx->state.transformMatrix = (MATRIX4){
        .v = {
            a, b, 0, 0,
            c, d, 0, 0,
            0, 0, 1, 0,
            e, f, 0, 1
        }
    };
}
// 样式设置
void canvas2d_set_stroke_style(canvas2d_context_t *ctx, COLOR32 color) {
    if (!ctx) return;
    ctx->state.strokeStyle = color;
}

void canvas2d_set_fill_style(canvas2d_context_t *ctx, COLOR32 color) {
    if (!ctx) return;
    ctx->state.fillStyle = color;
}

void canvas2d_set_line_width(canvas2d_context_t *ctx, float width) {
    if (!ctx) return;
    ctx->state.lineWidth = width;
}
// 路径操作函数
void canvas2d_begin_path(canvas2d_context_t *ctx) {
    if (!ctx) return;
    ctx->state.pathPointsCount = 0;
    ctx->state.pathOpen = true;
}

void canvas2d_move_to(canvas2d_context_t *ctx, float x, float y) {
    if (!ctx || !ctx->state.pathOpen) return;
    
    // 添加点到路径
    if (ctx->state.pathPointsCount >= ctx->state.pathPointsCapacity) {
        ctx->state.pathPointsCapacity = ctx->state.pathPointsCapacity ? ctx->state.pathPointsCapacity * 2 : 16;
        ctx->state.pathPoints = realloc(ctx->state.pathPoints, ctx->state.pathPointsCapacity * sizeof(VECTOR2));
    }
    
    ctx->state.pathPoints[ctx->state.pathPointsCount++] = (VECTOR2){NORM(x), NORM(y)};
}

void canvas2d_line_to(canvas2d_context_t *ctx, float x, float y) {
    canvas2d_move_to(ctx, x, y);
}

void canvas2d_close_path(canvas2d_context_t *ctx) {
    if (!ctx || ctx->state.pathPointsCount < 2) return;
    
    // 添加第一个点以闭合路径
    canvas2d_line_to(ctx, ctx->state.pathPoints[0].x, ctx->state.pathPoints[0].y);
    ctx->state.pathOpen = false;
}

void canvas2d_stroke_path(canvas2d_context_t *ctx) {
    if (!ctx || ctx->state.pathPointsCount < 2) return;
    
    // 绘制路径
    for (int i = 0; i < ctx->state.pathPointsCount - 1; i++) {
        VECTOR2 p1 = ctx->state.pathPoints[i];
        VECTOR2 p2 = ctx->state.pathPoints[i + 1];
        
        // 计算线段方向和长度
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float length = sqrt(dx * dx + dy * dy);
        
        if (length > 0) {
            dx /= length;
            dy /= length;
        }
        
        // 创建线段矩形
        float lineWidth = ctx->state.lineWidth;
        RECT lineRect = {
            p1.x - (dy * lineWidth) / 2,
            p1.y + (dx * lineWidth) / 2,
            length,
            lineWidth
        };
        
        // 计算旋转角度
        float angle = atan2(dy, dx);
        
        // 创建变换矩阵
        MATRIX4 transform;
        Matrix4_identity(&transform);
        
        // 平移到起点
        MATRIX4 translation;
        Matrix4_identity(&translation);
        Matrix4_translate(&translation, &(VECTOR3){NORM(p1.x), NORM(p1.y) , 0});
        Matrix4_multiply(&transform, &translation, &transform);
        
        // 旋转
        MATRIX4 rotation;
        Matrix4_identity(&rotation);
        Matrix4_rotate(&rotation, &(VECTOR3){0, 0, angle}, ROTATE_XYZ);
        Matrix4_multiply(&transform, &rotation, &transform);
        
        // 应用当前上下文变换
        Matrix4_multiply(&ctx->state.transformMatrix, &transform, &transform);
        
        RECT uv = {0, 0, 1, 1};
        DRAWIMAGE(tr.texture[TEX_WHITE], &lineRect, &uv, ctx->state.strokeStyle, &transform);
    }
}