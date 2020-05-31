#include "opengl_texture.cpp"
#include "opengl_text.cpp"

#include "opengl_renderer.h"

internal void RendererInit(Renderer *renderer){
    /* Quad Setup */
    glGenVertexArrays(1, &renderer->quad_vao);
    glBindVertexArray(renderer->quad_vao);
    
    //fill vbo with 0s
    glGenBuffers(1, &renderer->quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->quad_vertex_batch), NULL, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &renderer->quad_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->quad_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->quad_index_batch), NULL, GL_DYNAMIC_DRAW);
    
    renderer->quad_shader = CreateOpenGLShader("quad_vertex.glsl", "quad_fragment.glsl");
    glUseProgram(renderer->quad_shader);
    
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    //colour
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void *)(7 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    /* ------------- */
    
    /* Texture Setup */
    glGenVertexArrays(1, &renderer->texture_vao);
    glBindVertexArray(renderer->texture_vao);
    
    //fill vbo with 0s
    glGenBuffers(1, &renderer->texture_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->texture_vertex_batch), NULL, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &renderer->texture_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->texture_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->texture_index_batch), NULL, GL_DYNAMIC_DRAW);
    
    renderer->texture_shader = CreateOpenGLShader("texture_vertex.glsl", "texture_fragment.glsl");
    glUseProgram(renderer->texture_shader);
    
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    //colour
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void *)(7 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    /* ------------- */
    
    /* Text Setup */
    glGenVertexArrays(1, &renderer->text_vao);
    glBindVertexArray(renderer->text_vao);
    
    glGenBuffers(1, &renderer->text_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->text_vbo);
    //6 vertices of 4 floats each
    glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->text_vertex_batch), NULL, GL_DYNAMIC_DRAW);
    
    renderer->text_shader = CreateOpenGLShader("text_vertex.glsl", "text_fragment.glsl");
    glUseProgram(renderer->text_shader);
    
    //position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)0);
    glEnableVertexAttribArray(0);
    
    //color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(2 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 8, (void *)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    
    renderer->ortho = CreateOrthographicMat4(0, 1280, 0, 720, -1.0f, 100.0f);
    renderer->view = Mat4Identity();
    Mat4Translate(&renderer->view, Vec3(0, 0, -10));
    renderer->quads_submitted = 0;
    renderer->textures_submitted = 0;
    renderer->text_submitted = 0;
}

internal void RendererStart(Renderer *renderer, Vec2 camera_pos){
    glClearColor(0, 0.05, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Mat4Translate(&renderer->view, Vec3(-camera_pos.x, -camera_pos.y, -10));
    {   /* Quad Data */
        renderer->quads_submitted = 0;
        glUseProgram(renderer->quad_shader);
        glBindVertexArray(renderer->quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->quad_ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, 0);
    }
    {   /* Texture Data */
        renderer->textures_submitted = 0;
        glUseProgram(renderer->texture_shader);
        glBindVertexArray(renderer->texture_vao);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->texture_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->texture_ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, 0);
    }
    
    {   /* Text Data */
        renderer->text_submitted = 0;
        glUseProgram(renderer->text_shader);
        glBindVertexArray(renderer->text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
    }
}

internal void RendererDraw(Renderer *renderer){
    if(renderer->quads_submitted > 0){
        glUseProgram(renderer->quad_shader);
        glBindVertexArray(renderer->quad_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderer->quad_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->quads_submitted * sizeof(QUAD_DATA), renderer->quad_vertex_batch);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->quad_ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderer->quads_submitted * sizeof(QUAD_INDICES), renderer->quad_index_batch);
        
        ShaderLoadMat4(renderer->quad_shader, renderer->ortho, "ortho");
        ShaderLoadMat4(renderer->quad_shader, renderer->view, "view");
        
        glDrawElements(GL_TRIANGLES, renderer->quads_submitted * (sizeof(QUAD_INDICES)/sizeof(u32)), GL_UNSIGNED_INT, 0);
    }
    if(renderer->textures_submitted > 0){
        glBindTexture(GL_TEXTURE_2D, renderer->texture_current_id);
        glUseProgram(renderer->texture_shader);
        glBindVertexArray(renderer->texture_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderer->texture_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->textures_submitted * sizeof(QUAD_DATA), renderer->texture_vertex_batch);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->texture_ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderer->textures_submitted * sizeof(QUAD_INDICES), renderer->texture_index_batch);
        
        ShaderLoadMat4(renderer->texture_shader, renderer->ortho, "ortho");
        ShaderLoadMat4(renderer->texture_shader, renderer->view, "view");
        
        glDrawElements(GL_TRIANGLES, renderer->textures_submitted * (sizeof(QUAD_INDICES)/sizeof(u32)), GL_UNSIGNED_INT, 0);
    }
    if(renderer->text_submitted > 0){
        glBindTexture(GL_TEXTURE_2D, renderer->text_current_id);
        glUseProgram(renderer->text_shader);
        glBindVertexArray(renderer->text_vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, renderer->text_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, renderer->text_submitted * TEXT_BATCH_SIZE * sizeof(f32), renderer->text_vertex_batch);
        
        ShaderLoadMat4(renderer->text_shader, renderer->ortho, "ortho");
        glDrawArrays(GL_TRIANGLES, 0, renderer->text_submitted * TEXT_BATCH_SIZE);
    }
    renderer->quads_submitted = 0;
    renderer->textures_submitted = 0;
    renderer->text_submitted = 0;
}

internal void RendererSubmitQuad(Renderer *renderer, Mat4 trans, Vec4 color){
    if(renderer->quads_submitted >= MAX_QUADS_PER_DRAW){
        RendererDraw(renderer);
    }
    for(int i = 0; i < 4; i++){
        f32 vert_x = QUAD_DATA[i * 9];
        f32 vert_y = QUAD_DATA[i * 9 + 1];
        f32 vert_z = QUAD_DATA[i * 9 + 2];
        
        f32 color_r = QUAD_DATA[i * 9 + 3] * color.r;
        f32 color_g = QUAD_DATA[i * 9 + 4] * color.g;
        f32 color_b = QUAD_DATA[i * 9 + 5] * color.b;
        f32 color_a = QUAD_DATA[i * 9 + 6] * color.a;
        
        f32 texture_x = QUAD_DATA[i * 9 + 7];
        f32 texture_y = QUAD_DATA[i * 9 + 8];
        
        Vec4 vertex_pos = trans * Vec4(vert_x, vert_y, vert_z, 1.0f);
        
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9)] = vertex_pos.x;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 1] = vertex_pos.y;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 2] = vertex_pos.z;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 3] = color_r;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 4] = color_g;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 5] = color_b;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 6] = color_a;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 7] = texture_x;
        renderer->quad_vertex_batch[(36 * renderer->quads_submitted) + (i * 9) + 8] = texture_y;
    }
    
    for(int i = 0; i < 6; i++){
        renderer->quad_index_batch[(6 * renderer->quads_submitted) + i] = QUAD_INDICES[i] + (4 * renderer->quads_submitted);
    }
    
    renderer->quads_submitted++;
}

internal void RendererSubmitTexturedQuad(Renderer *renderer, Texture texture, Mat4 trans, Vec4 color, Vec4 section){
    if(renderer->textures_submitted >= MAX_TEXTURES_PER_DRAW){
        RendererDraw(renderer);
    }
    
    for(int i = 0; i < 4; i++){
        f32 vert_x = QUAD_DATA[i * 9];
        f32 vert_y = QUAD_DATA[i * 9 + 1];
        f32 vert_z = QUAD_DATA[i * 9 + 2];
        
        f32 color_r = QUAD_DATA[i * 9 + 3] * color.r;
        f32 color_g = QUAD_DATA[i * 9 + 4] * color.g;
        f32 color_b = QUAD_DATA[i * 9 + 5] * color.b;
        f32 color_a = QUAD_DATA[i * 9 + 6] * color.a;
        
        f32 texture_x = (section.x + (QUAD_DATA[i * 9 + 7] * section.z))/ texture.w;
        f32 texture_y = (section.y + (QUAD_DATA[i * 9 + 8] * section.w))/ texture.h;
        
        //printf("x:%f, y:%f\n", texture_x, texture_y);
        
        Vec4 vertex_pos = trans * Vec4(vert_x, vert_y, vert_z, 1.0f);
        
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9)] = vertex_pos.x;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 1] = vertex_pos.y;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 2] = vertex_pos.z;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 3] = color_r;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 4] = color_g;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 5] = color_b;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 6] = color_a;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 7] = texture_x;
        renderer->texture_vertex_batch[(36 * renderer->textures_submitted) + (i * 9) + 8] = texture_y;
    }
    
    for(int i = 0; i < 6; i++){
        renderer->texture_index_batch[(6 * renderer->textures_submitted) + i] = QUAD_INDICES[i] + (4 * renderer->textures_submitted);
    }
    renderer->textures_submitted++;
}

internal void RendererDrawQuad(Renderer *renderer, Vec4 position_and_scale, Vec4 color){
    Mat4 scale_mat = Mat4Identity();
    Mat4 translate_mat = Mat4Identity();
    
    Mat4Scale(&scale_mat, Vec3(position_and_scale.z, position_and_scale.w, 1.0f));
    Mat4Translate(&translate_mat, Vec3(position_and_scale.x, position_and_scale.y, 0.0f));
    Mat4 transform = translate_mat * scale_mat;
    RendererSubmitQuad(renderer, transform, color);
}

internal void RendererDrawQuad(Renderer *renderer, Vec3 position, Vec2 scale, Vec4 color){
    Mat4 scale_mat = Mat4Identity();
    Mat4 translate_mat = Mat4Identity();
    
    Mat4Scale(&scale_mat, Vec3(scale.x, scale.y, 1.0f));
    Mat4Translate(&translate_mat, Vec3(position.x, position.y, position.z));
    Mat4 transform = translate_mat * scale_mat;
    RendererSubmitQuad(renderer, transform, color);
}

internal void RendererDrawTexturedQuad(Renderer *renderer, Texture texture, Vec4 section, Vec2 position, float scale){
    Mat4 scale_mat = Mat4Identity();
    Mat4 translate_mat = Mat4Identity();
    
    Mat4Scale(&scale_mat, Vec3(section.z * scale, section.w * scale, 1.0f));
    Mat4Translate(&translate_mat, Vec3(position.x, position.y, 0.0f));
    Mat4 transform = translate_mat * scale_mat;
    
    renderer->texture_current_id = texture.id;
    RendererSubmitTexturedQuad(renderer, texture, transform, Vec4(0, 0, 0, 1), section);
}

internal void RendererDrawTexturedQuad(Renderer *renderer, Texture texture, Vec4 section, Vec3 position, float scale){
    Mat4 scale_mat = Mat4Identity();
    Mat4 translate_mat = Mat4Identity();
    
    Mat4Scale(&scale_mat, Vec3(section.z * scale, section.w * scale, 1.0f));
    Mat4Translate(&translate_mat, Vec3(position.x, position.y, position.z));
    Mat4 transform = translate_mat * scale_mat;
    
    renderer->texture_current_id = texture.id;
    RendererSubmitTexturedQuad(renderer, texture, transform, Vec4(0, 0, 0, 1), section);
}

internal void RendererDrawText(Renderer *renderer, Font *font, char *text, Vec2 pos, float scale, Vec4 color){
    renderer->text_current_id = font->texture.id;
    if(renderer->text_submitted + strlen(text) >= MAX_TEXT_PER_DRAW){
        RendererDraw(renderer);
    }
    
    //hardcoded, breaks w other fonts
    
    scale /= 57;
    u32 base_line = 53 * scale;
    
    char *t;
    for(t = text; *t != '\0'; t++){
        Vec2 size = Vec2(scale * font->width[*t-32], scale * font->height[*t-32]);
        Vec2 offset = Vec2(scale * font->x_offset[*t-32], scale * font->y_offset[*t-32]);
        f32 advance = scale * font->x_advance[*t-32];
        
        f32 tex_x0 = (f32)font->x[*t-32] / 512.0f;
        f32 tex_y1 = (f32)font->y[*t-32] / 512.0f;
        f32 tex_x1 = ((f32)font->x[*t-32] + (f32)font->width[*t-32]) / 512.0f;
        f32 tex_y0 = ((f32)font->y[*t-32] + (f32)font->height[*t-32]) / 512.0f;
        
        f32 temp_y = pos.y + (base_line - size.y);
        temp_y -= font->y_offset[*t-32] * scale;
        f32 vertices[TEXT_BATCH_SIZE] = {
            pos.x + offset.x, temp_y, color.r, color.g, color.b, color.a, tex_x0, tex_y0, //bottom left
            pos.x + offset.x, temp_y + size.y, color.r, color.g, color.b, color.a, tex_x0, tex_y1,
            pos.x + offset.x + size.x, (temp_y + size.y), color.r, color.g, color.b, color.a, tex_x1, tex_y1,
            pos.x + offset.x, temp_y, color.r, color.g, color.b, color.a, tex_x0, tex_y0,
            pos.x + offset.x + size.x, temp_y, color.r, color.g, color.b, color.a, tex_x1, tex_y0, //bottom right
            pos.x + offset.x + size.x, temp_y + size.y, color.r, color.g, color.b, color.a, tex_x1, tex_y1 //top right
        };
        
        //send to batch
        for(int i = 0; i < TEXT_BATCH_SIZE; i++){
            renderer->text_vertex_batch[(TEXT_BATCH_SIZE * renderer->text_submitted) + i] = vertices[i];
        }
        renderer->text_submitted++;
        pos.x += font->x_advance[*t-32] * scale;
    }
}

internal void RendererFinish(Renderer *renderer){
    RendererDraw(renderer);
    glViewport(0, 0, app_platform->width, app_platform->height);
    renderer->ortho = CreateOrthographicMat4(0, PROGRAM_WIDTH, 0, PROGRAM_HEIGHT, -1.0f, 100.0f);
}