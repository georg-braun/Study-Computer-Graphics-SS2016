#version 330
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable
// default130.vert: a simple vertex shader

layout(location = 0)uniform mat4 modelMatrix;
layout(location = 1)uniform mat4 viewlMatrix;
layout(location = 2)uniform mat4 perspectiveMatrix;
layout(location = 0)uniform mat3 normalMatrix;

layout(location = 0)in vec4 vert;
layout(location = 1)in vec4 texCoord;
layout(location = 2)in vec4 vertexNormal;

layout(location = 0)out vec4 col;
layout(location = 1)out vec4 texC;
layout(location = 0)out vec3 normal;
layout(location = 2)out vec3 position;

void main(void)
{
    normal      = normalize(normalMatrix*vertexNormal.xyz);
    position    = vec3( modelMatrix * vert);
    texC        = texCoord;
    gl_Position = perspectiveMatrix * viewlMatrix * modelMatrix * vert;
}



