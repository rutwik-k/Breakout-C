
#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#include "opengl_shader.cpp"

#define MAX_QUADS_PER_DRAW 5000
#define MAX_TEXTURES_PER_DRAW 5000
#define MAX_TEXT_PER_DRAW 5000

#define TEXT_BATCH_SIZE 6 * 4 * 4

const f32 QUAD_DATA[] = {
    //x     y     z   | r     g     b     a   |  u     v*/
    0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
    0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
    1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f
};

const u32 QUAD_INDICES[] = {
    0, 2, 3,
    0, 1, 3
};

struct Renderer{
    /* Quad Data */
    GLuint quad_vao;
    GLuint quad_vbo;
    GLuint quad_ibo;
    GLuint quad_shader;
    f32 quad_vertex_batch[36 * MAX_QUADS_PER_DRAW];
    u32 quad_index_batch[6 * MAX_QUADS_PER_DRAW];
    u32 quads_submitted;
    
    /* Texture Data */
    GLuint texture_vao;
    GLuint texture_vbo;
    GLuint texture_ibo;
    GLuint texture_shader;
    GLuint texture_current_id;
    f32 texture_vertex_batch[36 * MAX_TEXTURES_PER_DRAW];
    u32 texture_index_batch[6 * MAX_TEXTURES_PER_DRAW];
    u32 textures_submitted;
    
    /* Text Data */
    GLuint text_vao;
    GLuint text_vbo;
    GLuint text_shader;
    GLuint text_current_id;
    f32 text_vertex_batch[6 * 4 * MAX_TEXT_PER_DRAW];
    u32 text_submitted;
    
    Mat4 ortho;
    Mat4 view;
};

void RendererInit(Renderer *renderer);
void RendererStart(Renderer *renderer);
void RendererDrawQuad();
void RendererFinish();

#endif //OPENGL_RENDERER_H
