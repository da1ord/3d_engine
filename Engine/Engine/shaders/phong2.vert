#version 140

precision mediump float;

// Input to vertex shader
in vec4 inPosition;                 // Input vertex position
in vec3 inNormal;                   // Input vertex normal
in vec2 inTexCoord;                 // Input vertex texture coordinates
in vec3 inTangent;                  // Input vertex tangent
in vec3 inBinormal;                 // Input vertex binormal

// Matrices
uniform	mat4 MVP;                   // Model-View-Projection matrix
uniform	mat4 lightMVP;                   // Model-View-Projection matrix
uniform	mat4 M;                     // Model matrix
uniform	mat3 N;                     // Normal matrix

// Output to fragment shader
out vec4 varPosition;               // Vertex position	|WORLD SPACE|
smooth out vec3 varNormal;          // Normal		|WORLD SPACE|
out vec2 varTexCoord;        // Texture coords
out vec3 varTangent;         // Tangent
out vec3 varBinormal;        // Binormal
out vec4 varLightPos;

void main() {
    varTexCoord = inTexCoord;
    varNormal = N * inNormal;
    varTangent = N * inTangent;
    varBinormal = N * inBinormal;
    varPosition =  M * inPosition;
    varLightPos = lightMVP * inPosition;

    gl_Position = MVP * inPosition;
}
