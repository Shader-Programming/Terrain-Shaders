#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aUV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;


out vec2 UV;
out vec3 posWS ;
out vec3 normals;

vec3 CalculateGerstnerWaves(vec3 V, float amp, vec2 wavedir, float crestdist, float speed);

void main()
{
    UV = aUV;
    posWS = (model * vec4(aPos, 1.0)).xyz; 

    vec3 ModPos = CalculateGerstnerWaves(aPos, 1.5, vec2(1.0,0.0),50,0.5);
    
    gl_Position = projection * view * model*vec4(ModPos,1.0);
    normals = aNormals ;
}

vec3 CalculateGerstnerWaves(vec3 V, float amp, vec2 wavedir, float crestdist, float speed){
    speed*=time;
    float wavelength = (2*3.14152)/crestdist;
    float phasespd = sqrt(9.8)/wavelength; //phase speed

    float f = V.y+(dot(V.xy,wavedir)-phasespd*speed)*wavelength;
    V.x += wavedir.x*(amp*cos(f));
    V.y += amp*sin(f);
    V.z += wavedir.y*(amp*cos(f));

    return V;
}