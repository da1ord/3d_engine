#version 140
#extension GL_EXT_texture_array : enable

precision mediump float;

uniform sampler2DArray tex_diff;
uniform float level;

in vec3 normal;
in vec2 texCoord;

out vec4 outColor;

void main() {
  vec3 color = texture(tex_diff, vec3(texCoord, level)).rgb;
  outColor = vec4(color, 1.0f);
}
