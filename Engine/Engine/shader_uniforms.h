#ifndef TYPE_DEFS_H_
#define TYPE_DEFS_H_

struct ShaderUniforms {
  // Textures
  GLint tex_diff;
  GLint tex_norm;
  GLint tex_alpha;
  // Matrices
  GLint MVP;
  GLint M;
  GLint V_inv;
  GLint N;
  // Material
  GLint ambientColor;
  GLint diffuseColor;
  GLint specularColor;
  GLint shininess;
  GLint alpha;
  
  GLint time;
  
  GLint has_diff;
  GLint has_norm;
  GLint has_alpha;
};

#endif