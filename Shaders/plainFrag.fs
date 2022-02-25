#version 330 core
out vec4 FragColor ;


//in vec2 TexCoords ;
//in vec3 gWorldPos_FS_in ;
in vec3 posGS;
in vec3 normGS;
in float visibilityGS;

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
uniform float scale;
uniform vec3 sky;

uniform sampler2D rocktexture;
uniform sampler2D grasstexture;
uniform sampler2D sandtexture;
vec3 GetDirLight(vec3 norm, vec3 viewdir, vec3 heightcol);

void main()
{
    float height = posGS.y/scale;
    vec3 heightcol = vec3(0,0,0);

    vec3 topcol = vec3(0,0.6,0.46);
    vec3 medcol = vec3(0.26,0.27,0.29);
    vec3 lowcol = vec3(0.77,0.64,0.52);
    //percentage multiplier for each of the 3 axis
    vec3 blendpercent = normalize(abs(normGS));
    float b = (blendpercent.x+blendpercent.y+blendpercent.z);
    blendpercent = blendpercent/vec3(b);

    //blend the 3 values with the textures from each axis perspective
    vec4 xaxis = texture(rocktexture,posGS.yz);
    vec4 yaxis = texture(rocktexture,posGS.xz);
    vec4 zaxis = texture(rocktexture,posGS.xy);

    vec4 tptex = xaxis*blendpercent.x+yaxis*blendpercent.y+zaxis*blendpercent.z;

    if(height > 0.3){
        //textured
        //heightcol = vec3(mix(vec3(texture(rocktexture,posGS.xz)), vec3(texture(grasstexture,posGS.xz)),smoothstep(0.2,1.0,height)).rgb);
        //flat colour
        heightcol = vec3(mix(vec3(medcol), vec3(topcol),smoothstep(0.3,1.0,height)).rgb);
        
    }else{
        //textured
        //heightcol = vec3(mix(vec3(texture(sandtexture,posGS.xz)), vec3(texture(rocktexture,posGS.xz)),smoothstep(0.0,0.2,height)).rgb);
        //flat colour
        heightcol = vec3(mix(vec3(lowcol), vec3(medcol),smoothstep(0.0,0.3,height)).rgb);
        

        
    }

    //dunno how to use triplaner texturing

    vec3 result = vec3(0,0,0);

    vec3 viewdir = normalize(viewPos - posGS);
    vec3 norm = normalize(normGS);

    result = GetDirLight(norm,viewdir,heightcol);

    FragColor = vec4(result,1.0f) ;
    FragColor = mix(vec4(sky,1.0),FragColor,visibilityGS);
}

vec3 GetDirLight(vec3 norm, vec3 viewdir, vec3 heightcol){
    vec3 lightdir = normalize(-dirlight.direction);
    vec3 ambient = dirlight.ambient*heightcol;

    float diffusefactor = max(dot(norm,lightdir),0.0);
    vec3 diffuse = dirlight.diffuse*diffusefactor*heightcol;

    vec3 reflectdir = reflect(lightdir,norm);
    float spec = pow(max(dot(viewdir,reflectdir),0.0),mat.shininess);
    vec3 specular = dirlight.specular*(diffusefactor*mat.specular);

    return ambient+diffuse+specular;
}