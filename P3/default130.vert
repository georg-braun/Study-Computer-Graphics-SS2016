#version 130
// default130.vert: a simple vertex shader


uniform mat4 modelMatrix;
uniform mat4 viewlMatrix;
uniform mat4 perspectiveMatrix;
attribute vec4 vert;
attribute vec4 texCoord;
varying vec4 texC;
varying vec4 col;

void main(void)
{
    col = vec4(0.0, 1.0f, 0.0f, 0.0f ); // P3.5 - Set Fix Color //color;
    texC = texCoord;
    gl_Position = perspectiveMatrix * viewlMatrix * modelMatrix * vert;
}


/*
attribute vec4 qt_Vertex;
attribute vec4 qt_MultiTexCoord0;
uniform mat4 qt_ModelViewProjectionMatrix;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
}
*/
