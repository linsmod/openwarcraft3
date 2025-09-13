#include "r_local.h"
#include <unistd.h>

void R_PrintSysText2(LPCSTR string, DWORD x, DWORD y, COLOR32 color,LPMATRIX4 transform){
static VERTEX simp[256 * 6];
    LPVERTEX it = simp;
    DWORD string_len = string ? strlen(string) : 0;
    DWORD max_chars = sizeof(simp) / (6 * sizeof(VERTEX)); // 最多支持的字符数
    
    for (LPCSTR s = string; *s && (DWORD)(s - string) < max_chars && (DWORD)(s - string) < string_len; s++) {
        DWORD ch = *s;
        float fx = ch % 16;
        float fy = ch / 16;
        it = R_AddQuad(it, &(RECT ) {
            x + 10 * (s - string), y, 8, 16
        }, &(RECT ) {
            fx/16,fy/8,1.f/16,1.f/8
        }, color, 0);
    }
    
    DWORD num_vertices = (DWORD)(it - simp);
    size2_t window = R_GetWindowSize();
    MATRIX4 ui_matrix,model_matrix;
    Matrix4_ortho(&ui_matrix, 0.0f, window.width, window.height, 0.0f, 0.0f, 100.0f);
    
    if(!transform)
        Matrix4_identity(&model_matrix);
    else 
        model_matrix = *transform;

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glBindVertexArray, tr.buffer[RBUF_TEMP1]->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, tr.buffer[RBUF_TEMP1]->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(VERTEX) * num_vertices, simp, GL_STATIC_DRAW);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, ui_matrix.v);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uModelMatrix, 1, GL_FALSE, model_matrix.v);
    R_BindTexture(tr.texture[TEX_FONT], 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, num_vertices);
}

void R_PrintSysText(LPCSTR string, DWORD x, DWORD y, COLOR32 color) {
    return R_PrintSysText2(string,x,y,color,NULL);
}

void R_SetBlending(BLEND_MODE mode) {
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return;
//    switch (mode) {
//        case BLEND_MODE_NONE: R_Call(glBlendFunc, GL_ONE, GL_ZERO); break;
//        case BLEND_MODE_BLEND: R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
//        case BLEND_MODE_ALPHAKEY: R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
//        case BLEND_MODE_ADD: R_Call(glBlendFunc, GL_ONE, GL_ONE); break;
////        case AM_ADDALPHA: R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE); break;
//        case BLEND_MODE_MODULATE: R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
//        case BLEND_MODE_MODULATE_2X: R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
//    }
}

void R_DrawImageEx(LPCDRAWIMAGE drawImage) {
    VERTEX simp[6];
    R_AddQuad(simp, &drawImage->screen, &drawImage->uv, drawImage->color, 0);
    
    if (drawImage->rotate) {
        VECTOR2 tmp1 = simp[1].texcoord;
        VECTOR2 tmp2 = simp[5].texcoord;
        simp[1].texcoord = tmp2;
        simp[5].texcoord = tmp1;
    }

    LPCSHADER shader = tr.shader[drawImage->shader];
    
    //    size2_t screensize = R_GetWindowSize();
    MATRIX4 ui_matrix, model_matrix;
    Matrix4_ortho(&ui_matrix, 0.0f, 0.8, 0.6, 0.0f, 0.0f, 100.0f);

    // Matrix4_identity(&model_matrix);
    if(!drawImage->model_matrix)
    {
        Matrix4_identity(&model_matrix);
    }
    else{
        model_matrix = *drawImage->model_matrix;
    }
    
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glUseProgram, shader->progid);
    R_Call(glUniformMatrix4fv, shader->uViewProjectionMatrix, 1, GL_FALSE, ui_matrix.v);
    R_Call(glUniformMatrix4fv, shader->uModelMatrix, 1, GL_FALSE, model_matrix.v);
    R_Call(glUniform1f , shader->uActiveGlow, 1);
    R_Call(glBindVertexArray, tr.buffer[RBUF_TEMP1]->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, tr.buffer[RBUF_TEMP1]->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(VERTEX) * 6, simp, GL_STATIC_DRAW);
    R_Call(glEnable, GL_BLEND);
    
    R_SetBlending(drawImage->alphamode);
    R_BindTexture(drawImage->texture, 0);
    
//    R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (drawImage->uv.w > 1 || drawImage->uv.h > 1) {
        R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        R_Call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glDrawArrays, GL_TRIANGLES, 0, 6);

    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void R_DrawImage(LPCTEXTURE texture, LPCRECT screen, LPCRECT uv, COLOR32 color) {
    R_DrawImageEx(&MAKE(DRAWIMAGE,
                        .texture = texture,
                        .screen = *screen,
                        .uv = uv ? *uv : MAKE(RECT,0,0,1,1),
                        .color = color,
                        .rotate = false,
                        .shader = SHADER_UI,
                        .model_matrix = NULL
                    ));
}
void R_DrawImage2(LPCTEXTURE texture, LPCRECT screen, LPCRECT uv, COLOR32 color,LPMATRIX4 transform) {
    R_DrawImageEx(&MAKE(DRAWIMAGE,
                        .texture = texture,
                        .screen = *screen,
                        .uv = uv ? *uv : MAKE(RECT,0,0,1,1),
                        .color = color,
                        .rotate = false,
                        .shader = SHADER_UI,
                        .model_matrix = transform
                    ));
}

void R_DrawPic(LPCTEXTURE texture, float x, float y) {
    RECT screen = { x, y, texture->width / 2000.0, texture->height / 2000.0};
    R_DrawImage(texture, &screen, NULL, COLOR32_WHITE);
}

void R_DrawWireRect(LPCRECT rect, COLOR32 color) {
    static VERTEX simp[5];
    R_AddStrip(simp, rect, color);

    MATRIX4 ui_matrix;
    Matrix4_ortho(&ui_matrix, 0.0f, 0.8, 0.6, 0.0f, 0.0f, 100.0f);

    R_Call(glUseProgram, tr.shader[SHADER_UI]->progid);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_UI]->uViewProjectionMatrix, 1, GL_FALSE, ui_matrix.v);
    R_Call(glBindVertexArray, tr.buffer[RBUF_TEMP1]->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, tr.buffer[RBUF_TEMP1]->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(simp), simp, GL_STATIC_DRAW);
    
    R_BindTexture(tr.texture[TEX_WHITE], 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINE_STRIP, 0, sizeof(simp) / sizeof(*simp));
}

void R_DrawSelectionRect(LPCRECT rect, COLOR32 color) {
    size2_t const window = R_GetWindowSize();
    RECT screen = {
        rect->x * 0.8 / window.width,
        rect->y * 0.6 / window.height,
        rect->w * 0.8 / window.width,
        rect->h * 0.6 / window.height,
    };
    R_DrawWireRect(&screen, color);
}

void R_DrawBoundingBox(LPCBOX3 box, LPCMATRIX4 matrix, COLOR32 color) {
    static VERTEX simp[24];
    R_AddWireBox(simp, box, color);

    R_Call(glUseProgram, tr.shader[SHADER_DEFAULT]->progid);
    R_Call(glUniformMatrix4fv, tr.shader[SHADER_DEFAULT]->uModelMatrix, 1, GL_FALSE, matrix->v);
    R_Call(glBindVertexArray, tr.buffer[RBUF_TEMP1]->vao);
    R_Call(glBindBuffer, GL_ARRAY_BUFFER, tr.buffer[RBUF_TEMP1]->vbo);
    R_Call(glBufferData, GL_ARRAY_BUFFER, sizeof(simp), simp, GL_STATIC_DRAW);
    
    R_BindTexture(tr.texture[TEX_WHITE], 0);
    
    R_Call(glDisable, GL_CULL_FACE);
    R_Call(glEnable, GL_BLEND);
    R_Call(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    R_Call(glDrawArrays, GL_LINE_STRIP, 0, sizeof(simp) / sizeof(*simp));
}
void R_UpdateTransformMatrix2D(MATRIX4 transform,VECTOR2 position, float rotation, VECTOR2 scale, VECTOR2 pivot, RECT screen) {
    
    // 计算枢轴点的世界坐标
    float pivotX = screen.x + screen.w * pivot.x;
    float pivotY = screen.y + screen.h * pivot.y;
    
    // 构建变换矩阵：T * R * S * T⁻¹
    MATRIX4 translate_to_origin, scale_mat, rotate_mat, translate_to_target;
    VECTOR3 translate_vec, scale_vec, rotate_axis;
    QUATERNION quat;
    
    // 1. 平移到原点（相对于枢轴点）
    translate_vec = (VECTOR3){ -pivotX, -pivotY, 0.0f };
    Matrix4_identity(&translate_to_origin);
    Matrix4_translate(&translate_to_origin, &translate_vec);
    
    // 2. 应用缩放
    scale_vec = (VECTOR3){ scale.x, scale.y, 1.0f };
    Matrix4_identity(&scale_mat);
    Matrix4_scale(&scale_mat, &scale_vec);
    
    // 3. 应用旋转（使用四元数创建绕Z轴旋转）
    quat = (QUATERNION){ 0.0f, 0.0f, sinf(rotation / 2.0f), cosf(rotation / 2.0f) };
    Matrix4_identity(&rotate_mat);
    Matrix4_rotateQuat(&rotate_mat, &quat);
    
    // 4. 平移到目标位置
    translate_vec = (VECTOR3){ pivotX + position.x, pivotY + position.y, 0.0f };
    Matrix4_identity(&translate_to_target);
    Matrix4_translate(&translate_to_target, &translate_vec);
    
    // 组合变换：T_final * R * S * T_origin
    Matrix4_multiply(&translate_to_target, &rotate_mat, &transform);
    Matrix4_multiply(&transform, &scale_mat, &transform);
    Matrix4_multiply(&transform, &translate_to_origin, &transform);
}
// 创建2D变换矩阵（针对UI元素的简化版本）
MATRIX4 R_CreateTransformMatrix2D(VECTOR2 position, float rotation, VECTOR2 scale, VECTOR2 pivot, RECT screen) {
    MATRIX4 transform;
    Matrix4_identity(&transform);
    
    // 计算枢轴点的世界坐标
    float pivotX = screen.x + screen.w * pivot.x;
    float pivotY = screen.y + screen.h * pivot.y;
    
    // 构建变换矩阵：T * R * S * T⁻¹
    MATRIX4 translate_to_origin, scale_mat, rotate_mat, translate_to_target;
    VECTOR3 translate_vec, scale_vec, rotate_axis;
    QUATERNION quat;
    
    // 1. 平移到原点（相对于枢轴点）
    translate_vec = (VECTOR3){ -pivotX, -pivotY, 0.0f };
    Matrix4_identity(&translate_to_origin);
    Matrix4_translate(&translate_to_origin, &translate_vec);
    
    // 2. 应用缩放
    scale_vec = (VECTOR3){ scale.x, scale.y, 1.0f };
    Matrix4_identity(&scale_mat);
    Matrix4_scale(&scale_mat, &scale_vec);
    
    // 3. 应用旋转（使用四元数创建绕Z轴旋转）
    quat = (QUATERNION){ 0.0f, 0.0f, sinf(rotation / 2.0f), cosf(rotation / 2.0f) };
    Matrix4_identity(&rotate_mat);
    Matrix4_rotateQuat(&rotate_mat, &quat);
    
    // 4. 平移到目标位置
    translate_vec = (VECTOR3){ pivotX + position.x, pivotY + position.y, 0.0f };
    Matrix4_identity(&translate_to_target);
    Matrix4_translate(&translate_to_target, &translate_vec);
    
    // 组合变换：T_final * R * S * T_origin
    Matrix4_multiply(&translate_to_target, &rotate_mat, &transform);
    Matrix4_multiply(&transform, &scale_mat, &transform);
    Matrix4_multiply(&transform, &translate_to_origin, &transform);
    
    return transform;
}

// 简化的旋转矩阵创建函数
MATRIX4 R_CreateRotationMatrixZ(float angle) {
    MATRIX4 rotation;
    Matrix4_identity(&rotation);
    
    float c = cosf(angle);
    float s = sinf(angle);
    
    rotation.v[0] = c;
    rotation.v[1] = s;
    rotation.v[4] = -s;
    rotation.v[5] = c;
    
    return rotation;
}

// 简化的平移矩阵创建函数
MATRIX4 R_CreateTranslationMatrix2D(float x, float y) {
    MATRIX4 translation;
    Matrix4_identity(&translation);
    
    VECTOR3 translate_vec = { x, y, 0.0f };
    Matrix4_translate(&translation, &translate_vec);
    
    return translation;
}

// 简化的缩放矩阵创建函数
MATRIX4 R_CreateScaleMatrix2D(float sx, float sy) {
    MATRIX4 scale;
    Matrix4_identity(&scale);
    
    VECTOR3 scale_vec = { sx, sy, 1.0f };
    Matrix4_scale(&scale, &scale_vec);
    
    return scale;
}
