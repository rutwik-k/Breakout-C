#version 330 core
out vec4 frag_color;

in vec4 out_color;
in vec2 out_texcoords;

uniform sampler2D text;

const float width = 0.5;
const float edge = 0.1;

float alpha_value;

void main()
{
    float distance = 1.0 - texture(text, out_texcoords).a;
    alpha_value = 1.0 - smoothstep(width, width + edge, distance);
    frag_color = vec4(out_color.xyz, alpha_value * out_color.a);
}