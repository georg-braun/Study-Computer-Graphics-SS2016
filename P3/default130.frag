#version 130
// default130.frag: a simple fragment shader

varying vec4 col;
varying vec4 texC;
uniform sampler2D texture;
void main()
{

    //gl_FragColor = col ;
    gl_FragColor = texture2D(texture,vec2(texC.x,texC.y));
    //gl_FragColor = ve4(1.0f, 1.0f, 0.0f, 1.0f);
}

/*
uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
*/
