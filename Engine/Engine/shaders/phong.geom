#version 330

//points, lines, triangles, lines_adjacency, triangles_adjacency
layout (triangles) in;
//points, line_strip, triangle_strip
layout (triangle_strip, max_vertices = 7) out;
 
struct Variable {
  vec4 varPosition;     // Vertex position	|WORLD SPACE|
  vec3 varNormal;		    // Normal			|WORLD SPACE|
  vec2 varTexCoord;	    // Texture coords
  vec3 varTangent;		  // Tangent
  vec3 varBinormal;	    // Binormal
  vec4 vPosition;	      // Binormal
};

// Matrices
uniform	mat4 MVP;				    // Model-View-Projection matrix

in Variable vars_gs[];
in float viewZ_gs[];

out Variable vars_fs;
out float viewZ_fs;

void main () {
  vars_fs = vars_gs[0];
  viewZ_fs = viewZ_gs[0];
  
  for (int i = 0; i < gl_in.length(); i++) {
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();

  for (int i = 0; i < 1; i++) {
    vec4 y1 = MVP * vec4(0, 100, 0, 0);
    vec4 y2 = MVP * vec4(0, 102, 0, 0);
    gl_Position = gl_in[i].gl_Position;
    //gl_Position.y += 100.5;
    gl_Position += y1;
    EmitVertex();
    gl_Position = gl_in[i].gl_Position;
    //gl_Position.y += 102.5;
    gl_Position += y2;
    EmitVertex();
    gl_Position = gl_in[i + 1].gl_Position;
    //gl_Position.y += 100.5;
    gl_Position += y1;
    EmitVertex();
    gl_Position = gl_in[i + 1].gl_Position;
    //gl_Position.y += 102.5;
    gl_Position += y2;
    EmitVertex();
  }
  EndPrimitive();
  gl_PrimitiveID;
}