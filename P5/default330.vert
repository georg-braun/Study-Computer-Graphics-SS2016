# version 330
# extension GL_ARB_explicit_uniform_location : enable
# extension GL_ARB_separate_shader_objects : enable
// default330.vert : a simple vertex shader
//
// notes:
// GL_ARB_explicit_uniform_location is not needed for version >=430
// GL_ARB_separate_shader_objects is not needed for version >= 410

layout(location=0)uniform mat4 matrix;
layout(location=0)in vec4 vert;
layout(location=1)in vec4 texCoord;
layout(location=0)out vec4 col;
layout(location=1)out vec4 texC;

void main()
{
    //col = color;
    col = vec4(0.0, 1.0f, 0.0f, 0.0f ); // P3.5 - Set Fix Color //color;
    texC = texCoord ;
    gl_Position = matrix * vert;
}
