// canvas2d.c
#include "canvas2d.h"


inline HANDLE MemAlloc(long size) {
    HANDLE mem = malloc(size);
//    printf("Alloc (%d) %llx\n", size, mem);
    memset(mem, 0, size);
    return mem;
}

inline void MemFree(HANDLE mem) {
//    printf("Free %llx\n", mem);
    free(mem);
}

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
    canvas->initialized = false;
    
    // Create context
    canvas->context = (canvas2d_context_t*)MemAlloc(sizeof(canvas2d_context_t));
    if (!canvas->context) {
        canvas2d_log_error("Failed to allocate memory for context");
        MemFree(canvas);
        return NULL;
    }

    // Initialize context
    canvas->context->canvas = canvas;
    canvas->context->state.strokeStyle.r = 0;
    canvas->context->state.strokeStyle.g = 0;
    canvas->context->state.strokeStyle.b = 0;
    canvas->context->state.strokeStyle.a = 1;
    canvas->context->state.fillStyle.r = 0;
    canvas->context->state.fillStyle.g = 0;
    canvas->context->state.fillStyle.b = 0;
    canvas->context->state.fillStyle.a = 1;
    canvas->context->state.lineWidth = 1;
    canvas->context->state.lineCap = CANVAS2D_BUTT;
    canvas->context->state.lineJoin = CANVAS2D_MITER_JOIN;
    canvas->context->state.miterLimit = 10;
    canvas->context->state.globalCompositeOperation = CANVAS2D_SOURCE_OVER;
    canvas->context->state.repeatMode = CANVAS2D_REPEAT;
    canvas->context->state.patternImage = NULL;
    
    // Initialize transform matrix
    canvas->context->transformMatrix[0] = 1; // scale x
    canvas->context->transformMatrix[1] = 0; // skew y
    canvas->context->transformMatrix[2] = 0; // skew x
    canvas->context->transformMatrix[3] = 1; // scale y
    canvas->context->transformMatrix[4] = 0; // translate x
    canvas->context->transformMatrix[5] = 0; // translate y
    
    // Initialize state stack
    canvas->context->stateStack = (canvas2d_state_t*)MemAlloc(sizeof(canvas2d_state_t) * 10);
    canvas->context->stateStackSize = 0;
    canvas->context->stateStackCapacity = 10;
    
    canvas->context->pathStarted = false;
    canvas->context->currentPath = NULL;

    // Create render buffer
    canvas->buffer = R_MakeVertexArrayObject(NULL, 0);
    if (!canvas->buffer) {
        canvas2d_log_error("Failed to create render buffer");
        MemFree(canvas->context->stateStack);
        MemFree(canvas->context);
        MemFree(canvas);
        return NULL;
    }

    // Create texture
    canvas->texture = R_AllocateTexture(width, height);
    if (!canvas->texture) {
        canvas2d_log_error("Failed to create texture");
        R_ReleaseVertexArrayObject(canvas->buffer);
        MemFree(canvas->context->stateStack);
        MemFree(canvas->context);
        MemFree(canvas);
        return NULL;
    }

    canvas->initialized = true;
    return canvas;
}

void canvas2d_destroy(canvas2d_t *canvas) {
    if (!canvas) return;

    if (canvas->context) {
        if (canvas->context->stateStack) {
            MemFree(canvas->context->stateStack);
        }
        MemFree(canvas->context);
    }

    if (canvas->buffer) {
        R_ReleaseVertexArrayObject(canvas->buffer);
    }

    if (canvas->texture) {
        R_ReleaseTexture(canvas->texture);
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

void canvas2d_resize(canvas2d_t *canvas, int width, int height) {
    if (!canvas || !canvas->initialized) {
        canvas2d_log_error("Canvas not initialized");
        return;
    }

    canvas->width = width;
    canvas->height = height;

    // Recreate texture
    if (canvas->texture) {
        R_ReleaseTexture(canvas->texture);
    }
    canvas->texture = R_AllocateTexture(width, height);
    if (!canvas->texture) {
        canvas2d_log_error("Failed to recreate texture");
    }
}

// Context operations
void canvas2d_clear_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    // Clear the specified rectangle
    // Implementation would depend on the underlying renderer
    // For now, we'll just clear the entire canvas
    VERTEX vertices[6];
    R_AddQuad(vertices, &(RECT){0, 0, ctx->canvas->width, ctx->canvas->height}, 
              &(RECT){0, 0, 1, 1}, COLOR32_BLACK, 0);

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, 6);
}

void canvas2d_fill_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    VERTEX vertices[6];
    R_AddQuad(vertices, &(RECT){x, y, width, height}, 
              &(RECT){0, 0, 1, 1}, color, 0);

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, 6);
}

void canvas2d_stroke_rect(canvas2d_context_t *ctx, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.strokeStyle.r * 255),
        (BYTE)(ctx->state.strokeStyle.g * 255),
        (BYTE)(ctx->state.strokeStyle.b * 255),
        (BYTE)(ctx->state.strokeStyle.a * 255)
    };

    // Create vertices for rectangle outline
    VERTEX vertices[20]; // 5 vertices per line, 4 lines
    VERTEX *current = vertices;

    // Top line
    current = R_AddStrip(current, &(RECT){x, y, width, ctx->state.lineWidth}, color);
    
    // Right line
    current = R_AddStrip(current, &(RECT){x + width - ctx->state.lineWidth, y, ctx->state.lineWidth, height}, color);
    
    // Bottom line
    current = R_AddStrip(current, &(RECT){x, y + height - ctx->state.lineWidth, width, ctx->state.lineWidth}, color);
    
    // Left line
    current = R_AddStrip(current, &(RECT){x, y, ctx->state.lineWidth, height}, color);

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINE_STRIP, 0, 20);
}

// Circle drawing functions
void canvas2d_fill_circle(canvas2d_context_t *ctx, float x, float y, float radius) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    // Create vertices for circle (approximated with triangles)
    const int segments = 32;
    VERTEX vertices[segments * 3];
    VERTEX *current = vertices;

    for (int i = 0; i < segments; i++) {
        float angle1 = (i / (float)segments) * 2.0f * M_PI;
        float angle2 = ((i + 1) / (float)segments) * 2.0f * M_PI;

        // Center vertex
        current[0].position.x = x;
        current[0].position.y = y;
        current[0].position.z = 0;
        current[0].color = color;
        current[0].texcoord.x = 0;
        current[0].texcoord.y = 0;

        // Edge vertex 1
        current[1].position.x = x + cos(angle1) * radius;
        current[1].position.y = y + sin(angle1) * radius;
        current[1].position.z = 0;
        current[1].color = color;
        current[1].texcoord.x = 0;
        current[1].texcoord.y = 0;

        // Edge vertex 2
        current[2].position.x = x + cos(angle2) * radius;
        current[2].position.y = y + sin(angle2) * radius;
        current[2].position.z = 0;
        current[2].color = color;
        current[2].texcoord.x = 0;
        current[2].texcoord.y = 0;

        current += 3;
    }

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, segments * 3);
}

void canvas2d_stroke_circle(canvas2d_context_t *ctx, float x, float y, float radius) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.strokeStyle.r * 255),
        (BYTE)(ctx->state.strokeStyle.g * 255),
        (BYTE)(ctx->state.strokeStyle.b * 255),
        (BYTE)(ctx->state.strokeStyle.a * 255)
    };

    // Create vertices for circle outline (approximated with line segments)
    const int segments = 32;
    VERTEX vertices[segments * 2];
    VERTEX *current = vertices;

    for (int i = 0; i < segments; i++) {
        float angle1 = (i / (float)segments) * 2.0f * M_PI;
        float angle2 = ((i + 1) / (float)segments) * 2.0f * M_PI;

        // Vertex 1
        current[0].position.x = x + cos(angle1) * radius;
        current[0].position.y = y + sin(angle1) * radius;
        current[0].position.z = 0;
        current[0].color = color;
        current[0].texcoord.x = 0;
        current[0].texcoord.y = 0;

        // Vertex 2
        current[1].position.x = x + cos(angle2) * radius;
        current[1].position.y = y + sin(angle2) * radius;
        current[1].position.z = 0;
        current[1].color = color;
        current[1].texcoord.x = 0;
        current[1].texcoord.y = 0;

        current += 2;
    }

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINES, 0, segments * 2);
}

// Triangle drawing functions
void canvas2d_fill_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !triangle) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    VERTEX vertices[3];
    for (int i = 0; i < 3; i++) {
        vertices[i].position.x = triangle->points[i].x;
        vertices[i].position.y = triangle->points[i].y;
        vertices[i].position.z = 0;
        vertices[i].color = color;
        vertices[i].texcoord.x = 0;
        vertices[i].texcoord.y = 0;
    }

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, 3);
}

void canvas2d_stroke_triangle(canvas2d_context_t *ctx, const canvas2d_triangle_t *triangle) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !triangle) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.strokeStyle.r * 255),
        (BYTE)(ctx->state.strokeStyle.g * 255),
        (BYTE)(ctx->state.strokeStyle.b * 255),
        (BYTE)(ctx->state.strokeStyle.a * 255)
    };

    VERTEX vertices[6]; // 2 vertices per line, 3 lines
    VERTEX *current = vertices;

    // Line 1: point 0 to point 1
    current[0].position.x = triangle->points[0].x;
    current[0].position.y = triangle->points[0].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = triangle->points[1].x;
    current[1].position.y = triangle->points[1].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    current += 2;

    // Line 2: point 1 to point 2
    current[0].position.x = triangle->points[1].x;
    current[0].position.y = triangle->points[1].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = triangle->points[2].x;
    current[1].position.y = triangle->points[2].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    current += 2;

    // Line 3: point 2 to point 0
    current[0].position.x = triangle->points[2].x;
    current[0].position.y = triangle->points[2].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = triangle->points[0].x;
    current[1].position.y = triangle->points[0].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINES, 0, 6);
}

// Quad drawing functions
void canvas2d_fill_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !quad) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    VERTEX vertices[6];
    R_AddQuad(vertices, &(RECT){quad->points[0].x, quad->points[0].y, 
                                quad->points[2].x - quad->points[0].x, 
                                quad->points[2].y - quad->points[0].y}, 
              &(RECT){0, 0, 1, 1}, color, 0);

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, 6);
}

void canvas2d_stroke_quad(canvas2d_context_t *ctx, const canvas2d_quad_t *quad) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !quad) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.strokeStyle.r * 255),
        (BYTE)(ctx->state.strokeStyle.g * 255),
        (BYTE)(ctx->state.strokeStyle.b * 255),
        (BYTE)(ctx->state.strokeStyle.a * 255)
    };

    // Create vertices for quad outline
    VERTEX vertices[8]; // 2 vertices per line, 4 lines
    VERTEX *current = vertices;

    // Line 1: point 0 to point 1
    current[0].position.x = quad->points[0].x;
    current[0].position.y = quad->points[0].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = quad->points[1].x;
    current[1].position.y = quad->points[1].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    current += 2;

    // Line 2: point 1 to point 2
    current[0].position.x = quad->points[1].x;
    current[0].position.y = quad->points[1].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = quad->points[2].x;
    current[1].position.y = quad->points[2].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    current += 2;

    // Line 3: point 2 to point 3
    current[0].position.x = quad->points[2].x;
    current[0].position.y = quad->points[2].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = quad->points[3].x;
    current[1].position.y = quad->points[3].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    current += 2;

    // Line 4: point 3 to point 0
    current[0].position.x = quad->points[3].x;
    current[0].position.y = quad->points[3].y;
    current[0].position.z = 0;
    current[0].color = color;
    current[0].texcoord.x = 0;
    current[0].texcoord.y = 0;

    current[1].position.x = quad->points[0].x;
    current[1].position.y = quad->points[0].y;
    current[1].position.z = 0;
    current[1].color = color;
    current[1].texcoord.x = 0;
    current[1].texcoord.y = 0;

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINES, 0, 8);
}

// Path drawing functions (stub implementation)
void canvas2d_fill_path(canvas2d_context_t *ctx, const canvas2d_path_t *path) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !path) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    // For now, just draw lines between points
    if (path->count < 3) return;

    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    // Triangulate the path (simplified implementation)
    VERTEX *vertices = MemAlloc(path->count * 3 * sizeof(VERTEX));
    VERTEX *current = vertices;

    for (int i = 1; i < path->count - 1; i++) {
        // Triangle: point 0, point i, point i+1
        for (int j = 0; j < 3; j++) {
            canvas2d_point_t *point = NULL;
            if (j == 0) point = &path->points[0];
            else if (j == 1) point = &path->points[i];
            else point = &path->points[i + 1];

            current->position.x = point->x;
            current->position.y = point->y;
            current->position.z = 0;
            current->color = color;
            current->texcoord.x = 0;
            current->texcoord.y = 0;
            current++;
        }
    }

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, path->count * 3 * sizeof(VERTEX), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, path->count * 3);

    MemFree(vertices);
}

void canvas2d_stroke_path(canvas2d_context_t *ctx, const canvas2d_path_t *path) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !path) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    COLOR32 color = {
        (BYTE)(ctx->state.strokeStyle.r * 255),
        (BYTE)(ctx->state.strokeStyle.g * 255),
        (BYTE)(ctx->state.strokeStyle.b * 255),
        (BYTE)(ctx->state.strokeStyle.a * 255)
    };

    VERTEX *vertices = MemAlloc(path->count * 2 * sizeof(VERTEX));
    VERTEX *current = vertices;

    for (int i = 0; i < path->count - 1; i++) {
        // Line from point i to point i+1
        current[0].position.x = path->points[i].x;
        current[0].position.y = path->points[i].y;
        current[0].position.z = 0;
        current[0].color = color;
        current[0].texcoord.x = 0;
        current[0].texcoord.y = 0;

        current[1].position.x = path->points[i + 1].x;
        current[1].position.y = path->points[i + 1].y;
        current[1].position.z = 0;
        current[1].color = color;
        current[1].texcoord.x = 0;
        current[1].texcoord.y = 0;

        current += 2;
    }

    MATRIX4 matrix;
    Matrix4_ortho(&matrix, 0.0f, ctx->canvas->width, ctx->canvas->height, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, ctx->canvas->buffer->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, ctx->canvas->buffer->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, path->count * 2 * sizeof(VERTEX), vertices, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, matrix.v);
    
    R_BindTexture(ctx->canvas->texture, 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINES, 0, path->count * 2);

    MemFree(vertices);
}

// Text operations (simplified implementation)
void canvas2d_fill_text(canvas2d_context_t *ctx, const char *text, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !text) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    // Use the existing text rendering function
    COLOR32 color = {
        (BYTE)(ctx->state.fillStyle.r * 255),
        (BYTE)(ctx->state.fillStyle.g * 255),
        (BYTE)(ctx->state.fillStyle.b * 255),
        (BYTE)(ctx->state.fillStyle.a * 255)
    };

    R_PrintSysText(text, (DWORD)x, (DWORD)y, color);
}

void canvas2d_stroke_text(canvas2d_context_t *ctx, const char *text, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !text) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    // For stroke text, we'll use a slightly offset version to simulate stroke
    // This is a simplified implementation
    canvas2d_set_fill_style(ctx, ctx->state.strokeStyle.r, ctx->state.strokeStyle.g, 
                           ctx->state.strokeStyle.b, ctx->state.strokeStyle.a);
    
    // Draw text multiple times with small offsets to create stroke effect
    const float offset = ctx->state.lineWidth;
    for (float dx = -offset; dx <= offset; dx += 0.5f) {
        for (float dy = -offset; dy <= offset; dy += 0.5f) {
            if (dx == 0 && dy == 0) continue;
            R_PrintSysText(text, (DWORD)(x + dx), (DWORD)(y + dy), 
                          COLOR32_BLACK); // Black stroke
        }
    }
    
    // Draw the actual text on top
    canvas2d_set_fill_style(ctx, ctx->state.fillStyle.r, ctx->state.fillStyle.g, 
                           ctx->state.fillStyle.b, ctx->state.fillStyle.a);
    R_PrintSysText(text, (DWORD)x, (DWORD)y, 
                   COLOR32_WHITE); // White fill
}

void canvas2d_set_font(canvas2d_context_t *ctx, const char *fontFamily, float fontSize) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized) {
        canvas2d_log_error("Context or canvas not initialized");
        return;
    }

    // Load font using the existing font loading system
    if (fontFamily && strlen(fontFamily) > 0) {
        ctx->canvas->font = R_LoadFont(fontFamily, (DWORD)fontSize);
        if (!ctx->canvas->font) {
            canvas2d_log_error("Failed to load font");
        }
    }
}

// Image operations
canvas2d_image_t* canvas2d_create_image(const char *filename) {
    if (!filename) {
        canvas2d_log_error("Filename is null");
        return NULL;
    }

    canvas2d_image_t *image = (canvas2d_image_t*)MemAlloc(sizeof(canvas2d_image_t));
    if (!image) {
        canvas2d_log_error("Failed to allocate memory for image");
        return NULL;
    }

    strncpy(image->filename, filename, sizeof(image->filename) - 1);
    image->filename[sizeof(image->filename) - 1] = '\0';
    image->loaded = false;
    image->data = NULL;
    image->texture = NULL;
    image->width = 0;
    image->height = 0;
    image->channels = 4; // Default to RGBA

    // Load texture using the existing texture loading system
    image->texture = R_LoadTexture(filename);
    if (image->texture) {
        image->loaded = true;
        image->width = image->texture->width;
        image->height = image->texture->height;
    } else {
        canvas2d_log_error("Failed to load texture");
    }

    return image;
}

canvas2d_image_t* canvas2d_create_image_from_data(unsigned char *data, int width, int height, int channels) {
    if (!data || width <= 0 || height <= 0 || channels <= 0) {
        canvas2d_log_error("Invalid image data parameters");
        return NULL;
    }

    canvas2d_image_t *image = (canvas2d_image_t*)MemAlloc(sizeof(canvas2d_image_t));
    if (!image) {
        canvas2d_log_error("Failed to allocate memory for image");
        return NULL;
    }

    image->filename[0] = '\0';
    image->loaded = true;
    image->data = data;
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->texture = NULL;

    // Create texture from data
    image->texture = R_AllocateTexture(width, height);
    if (image->texture) {
        // Load mip level with the provided data
        R_LoadTextureMipLevel(image->texture, 0, (COLOR32*)data, width, height);
    } else {
        canvas2d_log_error("Failed to create texture from data");
        MemFree(image);
        return NULL;
    }

    return image;
}

void canvas2d_destroy_image(canvas2d_image_t *image) {
    if (!image) return;

    if (image->texture) {
        R_ReleaseTexture(image->texture);
    }

    if (image->data) {
        MemFree(image->data);
    }

    MemFree(image);
}

void canvas2d_draw_image(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !image || !image->loaded) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    DRAWIMAGE drawImage = {
        .texture = image->texture,
        .screen = {x, y, (float)image->width / 2000.0f, (float)image->height / 2000.0f},
        .uv = {0, 0, 1, 1},
        .color = COLOR32_WHITE,
        .rotate = false,
        .shader = SHADER_UI,
        .alphamode = BLEND_MODE_BLEND
    };

    R_DrawImageEx(&drawImage);
}

void canvas2d_draw_image_scaled(canvas2d_context_t *ctx, canvas2d_image_t *image, float x, float y, float width, float height) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !image || !image->loaded) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    DRAWIMAGE drawImage = {
        .texture = image->texture,
        .screen = {x, y, width, height},
        .uv = {0, 0, 1, 1},
        .color = COLOR32_WHITE,
        .rotate = false,
        .shader = SHADER_UI,
        .alphamode = BLEND_MODE_BLEND
    };

    R_DrawImageEx(&drawImage);
}

void canvas2d_draw_image_sliced(canvas2d_context_t *ctx, canvas2d_image_t *image, 
                               float sx, float sy, float sw, float sh, 
                               float dx, float dy, float dw, float dh) {
    if (!ctx || !ctx->canvas || !ctx->canvas->initialized || !image || !image->loaded) {
        canvas2d_log_error("Invalid parameters");
        return;
    }

    DRAWIMAGE drawImage = {
        .texture = image->texture,
        .screen = {dx, dy, dw, dh},
        .uv = {sx / image->width, sy / image->height, sw / image->width, sh / image->height},
        .color = COLOR32_WHITE,
        .rotate = false,
        .shader = SHADER_UI,
        .alphamode = BLEND_MODE_BLEND
    };

    R_DrawImageEx(&drawImage);
}

// Transform operations
void canvas2d_translate(canvas2d_context_t *ctx, float x, float y) {
    if (!ctx) return;

    // Apply translation to the transformation matrix
    ctx->transformMatrix[4] += x;
    ctx->transformMatrix[5] += y;
}

void canvas2d_rotate(canvas2d_context_t *ctx, float angle) {
    if (!ctx) return;

    float cos_a = cos(angle);
    float sin_a = sin(angle);

    // Create rotation matrix
    float matrix[6] = {
        cos_a, -sin_a,  // scale x, skew y
        sin_a, cos_a,   // skew x, scale y
        0, 0            // translate x, translate y
    };

    // Multiply current matrix by rotation matrix
    float result[6];
    result[0] = ctx->transformMatrix[0] * matrix[0] + ctx->transformMatrix[2] * matrix[1];
    result[1] = ctx->transformMatrix[1] * matrix[0] + ctx->transformMatrix[3] * matrix[1];
    result[2] = ctx->transformMatrix[0] * matrix[2] + ctx->transformMatrix[2] * matrix[3];
    result[3] = ctx->transformMatrix[1] * matrix[2] + ctx->transformMatrix[3] * matrix[3];
    result[4] = ctx->transformMatrix[4];
    result[5] = ctx->transformMatrix[5];

    memcpy(ctx->transformMatrix, result, sizeof(result));
}

void canvas2d_scale(canvas2d_context_t *ctx, float x, float y) {
    if (!ctx) return;

    // Apply scaling to the transformation matrix
    ctx->transformMatrix[0] *= x;  // scale x
    ctx->transformMatrix[1] *= y;  // skew y
    ctx->transformMatrix[2] *= x;  // skew x
    ctx->transformMatrix[3] *= y;  // scale y
}

void canvas2d_reset_transform(canvas2d_context_t *ctx) {
    if (!ctx) return;

    // Reset transformation matrix to identity
    ctx->transformMatrix[0] = 1; // scale x
    ctx->transformMatrix[1] = 0; // skew y
    ctx->transformMatrix[2] = 0; // skew x
    ctx->transformMatrix[3] = 1; // scale y
    ctx->transformMatrix[4] = 0; // translate x
    ctx->transformMatrix[5] = 0; // translate y
}

void canvas2d_save(canvas2d_context_t *ctx) {
    if (!ctx) return;

    // Save current state to stack
    if (ctx->stateStackSize >= ctx->stateStackCapacity) {
        // Resize stack if needed
        ctx->stateStackCapacity *= 2;
        ctx->stateStack = (canvas2d_state_t*)MemAlloc(ctx->stateStackCapacity * sizeof(canvas2d_state_t));
    }

    // Copy current state to stack
    memcpy(&ctx->stateStack[ctx->stateStackSize], &ctx->state, sizeof(canvas2d_state_t));
    ctx->stateStackSize++;
}

void canvas2d_restore(canvas2d_context_t *ctx) {
    if (!ctx || ctx->stateStackSize <= 0) return;

    // Restore state from stack
    ctx->stateStackSize--;
    memcpy(&ctx->state, &ctx->stateStack[ctx->stateStackSize], sizeof(canvas2d_state_t));
}

// Style operations
void canvas2d_set_stroke_style(canvas2d_context_t *ctx, float r, float g, float b, float a) {
    if (!ctx) return;

    ctx->state.strokeStyle.r = r;
    ctx->state.strokeStyle.g = g;
    ctx->state.strokeStyle.b = b;
    ctx->state.strokeStyle.a = a;
}

void canvas2d_set_fill_style(canvas2d_context_t *ctx, float r, float g, float b, float a) {
    if (!ctx) return;

    ctx->state.fillStyle.r = r;
    ctx->state.fillStyle.g = g;
    ctx->state.fillStyle.b = b;
    ctx->state.fillStyle.a = a;
}

void canvas2d_set_line_width(canvas2d_context_t *ctx, float width) {
    if (!ctx) return;

    ctx->state.lineWidth = width;
}

void canvas2d_set_line_cap(canvas2d_context_t *ctx, canvas2d_line_cap_t cap) {
    if (!ctx) return;

    ctx->state.lineCap = cap;
}

void canvas2d_set_line_join(canvas2d_context_t *ctx, canvas2d_line_join_t join) {
    if (!ctx) return;

    ctx->state.lineJoin = join;
}

void canvas2d_set_global_composite_operation(canvas2d_context_t *ctx, canvas2d_global_composite_operation_t operation) {
    if (!ctx) return;

    ctx->state.globalCompositeOperation = operation;
}