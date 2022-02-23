
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec2 TexCoords;
out vec3 posVS ;



void main()
{
    TexCoords = aTexCoords;  
    posVS = (model * vec4(aPos, 1.0)).xyz; 
}