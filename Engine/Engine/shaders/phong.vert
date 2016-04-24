#version 330

//precision mediump float;

// Input to vertex shader
in vec4 inPosition;				  // Input vertex position
in vec3 inNormal;				  // Input vertex normal
in vec2 inTexCoord;				  // Input vertex texture coordinates
in vec3 inTangent;				  // Input vertex tangent
in vec3 inBinormal;				  // Input vertex binormal

// Matrices
uniform	mat4 MVP;				  // Model-View-Projection matrix
uniform	mat4 M;					  // Model matrix
uniform	mat3 N;					  // Normal matrix
uniform	mat4 shadowMVP[3];        // Shadow map matrices

uniform vec3 farPlanes;

// Output to fragment shader
struct Variables {
  vec4 varPosition;				  // Vertex position	|WORLD SPACE|
  vec3 varNormal;		          // Normal			|WORLD SPACE|
  vec2 varTexCoord;               // Texture coords
  vec3 varTangent;				  // Tangent
  vec3 varBinormal;				  // Binormal
  vec4 vPosition;	              // Binormal
};
out Variables vars_fs;

out float viewZ_fs;

void main() {
  vars_fs.varTexCoord = inTexCoord;
  vars_fs.varNormal = N * inNormal;
  vars_fs.varTangent = inTangent;
  vars_fs.varBinormal = inBinormal;
  vars_fs.varPosition =  M * inPosition;
  vars_fs.vPosition = inPosition;

  gl_Position = MVP * inPosition;
  viewZ_fs = gl_Position.z;
}