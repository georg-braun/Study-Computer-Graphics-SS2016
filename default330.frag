#version 330
#extension GL_ARB_separate_shader_objects: enable

layout(location=0)in vec4 col;
layout(location=1)in vec4 texC;
layout(location=0)out vec4 fragColor;
layout(location=0)uniform sampler2D texture;

void main()
{
    fragColor = texture2D(texture,vec2(texC.x,texC.y));
}
