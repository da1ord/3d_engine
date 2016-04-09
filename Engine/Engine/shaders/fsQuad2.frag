#version 330

uniform sampler2D tex_diff;

in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main() {
  vec3 color = texture(tex_diff, texCoord).rgb;
  outColor = vec4(color, 1.0f);
}
