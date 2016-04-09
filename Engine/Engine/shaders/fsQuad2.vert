#version 330

in vec4 inPosition;
in vec2 inTexCoord;

out vec2 texCoord;

uniform float scale;
uniform vec2 offset;

void main() {
  texCoord = inPosition.xy * 0.5 + 0.5;
  vec4 pos = vec4(scale * inPosition.x, scale * inPosition.y, 
    inPosition.z, inPosition.w) + vec4(offset, 0.0, 0.0);
  gl_Position = pos;
}
