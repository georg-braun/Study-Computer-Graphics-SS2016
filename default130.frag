#version 130
// default130.frag: a simple fragment shader

varying vec4 col;

void main()
{
    gl_FragColor = col ;
}

/*
uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
*/
