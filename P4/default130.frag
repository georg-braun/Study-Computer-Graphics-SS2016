#version 330
#extension GL_ARB_separate_shader_objects : enable
// default130.frag: a simple fragment shader


layout(location = 0)uniform sampler2D texture;
layout(location = 0)uniform vec3 ka;           // ambient
layout(location = 1)uniform vec3 kd;           // diffuse
layout(location = 1)uniform vec3 ks;           // specular
layout(location = 0)uniform vec4 lightPosition;
layout(location = 1)uniform vec3 viewPosition;


layout(location = 0)in vec4 col;
layout(location = 1)in vec4 texC;
layout(location = 0)in vec3 normal;
layout(location = 2)in vec3 position;       // Fragment Position

layout(location = 0)out vec4 fragColor ;

void main()
{

    vec3 n          = normalize(normal);                            // Normale
    vec3 lightDir   = normalize(vec3(lightPosition)-position);
    vec3 viewDir    = normalize(viewPosition-position) ;
    vec3 reflectDir = reflect(-lightDir,n);                         // Light-Dir Vektor umdrehen (für reflect)


    vec3 diffuse = kd * max(dot(lightDir,n),0.0) ;
    vec3 spec    = ks * pow(max(dot(viewDir,reflectDir), 0.0), 4);


    vec3 outputColor = ka + diffuse + spec ;

    fragColor = vec4(outputColor,1.0) * texture2D(texture,vec2(texC.x,texC.y));


}


