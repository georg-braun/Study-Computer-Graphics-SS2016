#version 130
// default130.vert: a simple vertex shader


uniform mat4 matrix;
attribute vec4 vert;
attribute vec4 color;
varying vec4 col;

void main(void)
{
    col = color;
    gl_Position = matrix * vert;
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
