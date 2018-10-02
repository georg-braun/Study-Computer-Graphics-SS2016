#version 330
#extension GL_ARB_separate_shader_objects: enable

layout(location=0)in vec4 col;
layout(location=0)out vec4 fragColor;

void main()
{
    fragColor = col ;
}


/*
uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
*/
