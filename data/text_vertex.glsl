#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coords;

uniform mat4 ortho;

out vec4 out_color;
out vec2 out_texcoords;

void main()
{
    out_color = color;
    out_texcoords = tex_coords;
    gl_Position = ortho * vec4(pos.x, pos.y, 1.0, 1.0);
}