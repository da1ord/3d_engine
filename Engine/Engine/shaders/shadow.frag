#version 420

uniform sampler2D tex_alpha;      // Alpha texture
uniform float has_alpha_tex;      // Has alpha texture

in vec2 varTexCoord;

void main(void) {
  if (has_alpha_tex > 0) {
    float alpha = texture(tex_alpha, varTexCoord).r;
	if (alpha < 0.5f)
	{
	  discard;
	}
  }
}
