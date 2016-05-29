#version 330
#extension GL_ARB_separate_shader_objects: enable

layout(location=0)in vec4 col;
layout(location=0)out vec4 fragColor;

void main()
{
    fragColor = col ;
}
