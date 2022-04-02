#version 410 core

out vec4 FragColor ;

in vec3 normGS;
in vec3 posGS;
in vec2 TexCoordsGS;
in float visibilityGS;


uniform sampler2D normalmap;
uniform sampler2D DuDv;
uniform sampler2D reflection;
uniform sampler2D refraction;
uniform float time;
uniform vec3 lightdir;

uniform float screenW;
uniform float screenH;
uniform vec3 camerapos;
uniform vec3 sky;

vec3 Highlights(vec3 norms,vec3 viewDir);

float wavestrength = 0.02;
vec3 lightcolour = vec3(1.0,1.0,1.0);
float strength = 20;

void main(){
    vec3 viewDir = normalize(camerapos-posGS);

    vec2 UVScaler = vec2(TexCoordsGS)*30;
    vec2 D = (texture(DuDv,vec2(UVScaler.x+(time*0.02),UVScaler.y)).rg*2.0-1.0)*wavestrength;
    vec2 ndcXY = gl_FragCoord.xy;
    vec2 reflecttex = vec2((ndcXY.x)/screenW,(-ndcXY.y)/screenH);
    vec2 refracttex = vec2((ndcXY.x)/screenW,(ndcXY.y)/screenH);

    vec3 surfacenormal = vec3(0,1,0);
    vec3 waternormal = texture(normalmap,D).xyz;
    vec3 normal = normalize(vec3(waternormal.r*2.0-1.0,waternormal.b,waternormal.g*2.0-1.0));
    
    float fresnel = dot(viewDir,surfacenormal);

    vec4 reflect = texture(reflection,reflecttex+D);
    vec4 refract = texture(refraction,refracttex+D);


    vec4 result = vec4(vec3(mix(reflect,refract,fresnel)),1.0);
    result = result + vec4(Highlights(normal,viewDir),1.0);
    result += vec4(0.0,0.21,0.2,1.0);
    result = mix(vec4(sky,1.0),result,visibilityGS);
    FragColor = result;
}

vec3 Highlights(vec3 norms,vec3 viewDir){
    //Blinn Phong
    vec3 halfwaydir = normalize(lightdir+viewDir);
    float spec = pow(max(dot(norms,halfwaydir),0.0),1);
    vec3 specular = spec*lightcolour;
    //Phong
    //vec3 reflect = reflect(-lightdir,norms);
    //float spec = pow(max(dot(viewDir,reflect),0.0),16);  
    //vec3 specular = strength*spec*lightcolour;

    return specular;
}