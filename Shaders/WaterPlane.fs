#version 410 core

out vec4 FragColor ;

in vec3 normals;
in vec3 posWS;
in vec2 UV;

uniform sampler2D image;

void main(){
    vec3 result = texture(image,UV).rgb;
    FragColor = vec4(result,1.0);
}