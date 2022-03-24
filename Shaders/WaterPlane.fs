#version 410 core

out vec4 FragColor ;

in vec3 normals;
in vec3 posWS;
in vec2 UV;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform float screenW;
uniform float screenH;
uniform vec3 camerapos;

void main(){
    vec2 ndcXY = gl_FragCoord.xy;
    vec2 reflecttex = vec2((ndcXY.x)/screenW,(-ndcXY.y)/screenH);
    vec2 refracttex = vec2((ndcXY.x)/screenW,(ndcXY.y)/screenH);

    vec3 surfacenormal = vec3(0,1,0);
    vec3 viewDir = normalize(camerapos-posWS);
    
    float fresnel = dot(viewDir,surfacenormal);

    vec4 reflect = texture(reflection,reflecttex);
    vec4 refract = texture(refraction,refracttex);
    vec4 result = mix(reflect,refract,fresnel);
    FragColor = result;
}