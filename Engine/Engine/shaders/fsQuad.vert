#version 140

precision mediump float;

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;

uniform	mat4 MVP;
uniform float scale;
uniform vec2 offset;

void main() {
  normal = inNormal;
  texCoord = inTexCoord;
  vec4 pos = vec4(scale * inPosition.x, scale * inPosition.y, 
    inPosition.z, inPosition.w) + vec4(offset, 0.0, 0.0);
  gl_Position = pos;
}
