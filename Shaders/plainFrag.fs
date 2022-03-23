#version 330 core
out vec4 FragColor ;

in vec3 posGS;
in vec3 normGS;
in float visibilityGS;
in vec2 TexCoordsGS;

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
uniform sampler2D normalmap;

uniform sampler2D rocktexture;
uniform sampler2D grasstexture;
vec3 GetDirLight(vec3 norm, vec3 viewdir, vec3 heightcol);
vec3 TriPlaner(sampler2D newtex);
void main()
{
    //Normal Map Import
    //vec3 norm = normalize(texture(normalmap,TexCoordsGS).xyz);
    vec3 norm = normalize(normGS);

    //Height Dependant Texturing
    float height = posGS.y*scale;
    float flatness = norm.y*scale;
    vec3 heightcol = vec3(0,0,0);

    vec3 topcol = TriPlaner(rocktexture);
    vec3 lowcol = TriPlaner(grasstexture);

    //if(height > 1.0){
    //    heightcol = vec3(mix(medcol, topcol,smoothstep(1.0,1.8,height)).rgb);  
    //}else{
    //    heightcol = vec3(mix(lowcol, medcol,smoothstep(0.4,1.0,height)).rgb);       
    //}
    heightcol = vec3(mix(lowcol, topcol,smoothstep(0.5,1.0,height)).rgb); 
    if(flatness > 75){
        heightcol = vec3(mix(topcol, lowcol,smoothstep(75,80,flatness)).rgb); 
    }

    //Result
    vec3 result = vec3(0,0,0);
    vec3 viewdir = normalize(viewPos - posGS);

    //Directional Light
    result = GetDirLight(norm,viewdir,heightcol);

    //Fog Import
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

vec3 TriPlaner(sampler2D newtex){
    vec3 blendpercent = normalize(abs(normGS));
    float b = (blendpercent.x+blendpercent.y+blendpercent.z);
    blendpercent = blendpercent/vec3(b);

    vec4 xaxis = texture(newtex,posGS.yz);
    vec4 yaxis = texture(newtex,posGS.xz);
    vec4 zaxis = texture(newtex,posGS.xy);

    vec4 tptex = xaxis*blendpercent.x + yaxis*blendpercent.y + zaxis*blendpercent.z;
    return vec3(tptex);
}