#version 330

precision mediump float;

uniform sampler2D tex_diff;
uniform float level;

in vec3 normal;
in vec2 texCoord;

out vec4 outColor;

void main() {
  vec3 color = vec3(0.8, 0.8, 0.8);//texture(tex_diff, texCoord).rgb;
  outColor = vec4(color, 1.0f);
}
