#version 410 core

out vec4 FragColor ;

in vec3 normals;
in vec3 posWS;
in vec2 UV;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D DuDv;
uniform sampler2D normalmap;
uniform float time;
uniform vec3 lightdir;

uniform float screenW;
uniform float screenH;
uniform vec3 camerapos;

vec3 Highlights(vec3 norms,vec3 viewDir);

float wavestrength = 0.04;

void main(){
    vec3 viewDir = normalize(camerapos-posWS);

    vec2 D = (texture(DuDv,vec2(UV.x+(time*0.02),UV.y)).rg*2.0-1.0)*wavestrength;
    vec2 ndcXY = gl_FragCoord.xy;
    vec2 reflecttex = vec2((ndcXY.x)/screenW,(-ndcXY.y)/screenH);
    vec2 refracttex = vec2((ndcXY.x)/screenW,(ndcXY.y)/screenH);

    vec3 surfacenormal = vec3(0,1,0);
    vec3 waternormal = texture(normalmap,D).xyz;
    
    float fresnel = dot(viewDir,surfacenormal);

    vec4 reflect = texture(reflection,reflecttex+D);
    vec4 refract = texture(refraction,refracttex+D);


    vec4 result = ((mix(reflect,refract,fresnel))*(vec4(0.8,1.0,1.0,1.0))+vec4(Highlights(waternormal,viewDir),1.0));
    FragColor = result;
}

vec3 Highlights(vec3 norms,vec3 viewDir){
    vec3 halfwaydir = normalize(lightdir+viewDir);
    float spec = pow(max(dot(norms,halfwaydir),0.0),16);
    vec3 specular = vec3(1.0,1.0,1.0)*spec;
    return specular;
}