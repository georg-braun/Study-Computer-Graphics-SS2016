#version 330
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable
// sun130.vert: a simple vertex shader

layout(location = 0)uniform mat4 modelMatrix;
layout(location = 1)uniform mat4 viewlMatrix;
layout(location = 2)uniform mat4 perspectiveMatrix;

layout(location = 0)in vec4 vert;
layout(location = 1)in vec4 texCoord;

layout(location = 0)out vec4 texC;

void main(void)
{

    texC = texCoord;
    gl_Position = perspectiveMatrix * viewlMatrix * modelMatrix * vert;
}



