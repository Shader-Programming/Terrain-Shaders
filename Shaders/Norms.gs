#version 330 core
layout(triangles) in ;
layout(triangle_strip, max_vertices = 3) out ;
vec3 getNormal() ;

in vec3 posES[] ;
in vec2 TexCoordsES[];
in vec3 normES[];

out vec3 gWorldPos_FS_in ;
out vec3 normGS;
out vec2 TexCoordsGS;
vec3 GetSurfaceNormal();

void main()
{
   //would get surface normals here
   for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      gWorldPos_FS_in = posES[i] ;  
      TexCoordsGS = TexCoordsES[i];
      normGS = normES[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}