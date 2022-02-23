#version 330 core
layout(triangles) in ;
layout(triangle_strip, max_vertices = 3) out ;
vec3 getNormal() ;

in vec3 posES[] ;

out vec3 gWorldPos_FS_in ;
out vec3 gNorm;
vec3 GetSurfaceNormal();

void main()
{
   gNorm = GetSurfaceNormal();
   for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      gWorldPos_FS_in = posES[i] ;  
      EmitVertex() ;
  }
     EndPrimitive() ;

}

vec3 GetSurfaceNormal(){
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a,b));
}