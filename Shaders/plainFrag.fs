#version 330 core
out vec4 FragColor ;


//in vec2 TexCoords ;
in vec3 gWorldPos_FS_in ;
in vec2 TexCoordsGS;
in vec3 normGS;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};   

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

uniform DirLight dirlight;
uniform Material mat ;
uniform vec3 viewPos ;

void main()
{
    vec3 viewDir = normalize(viewPos - gWorldPos_FS_in);
    vec3 norm = normalize(normGS);
    vec3 lightdir = normalize(-dirlight.direction);

    vec3 ambient = dirlight.ambient*mat.ambient;

    float diff = max(dot(norm,lightdir),0.0);
    vec3 diffuse = dirlight.diffuse*(diff*mat.diffuse);

    vec3 reflectdir = reflect(lightdir,norm);
    float spec = pow(max(dot(viewDir,reflectdir),0.0),mat.shininess);
    vec3 specular = dirlight.specular*(diff*mat.specular);

    FragColor = vec4((ambient+diffuse+specular),1.0f) ;
}