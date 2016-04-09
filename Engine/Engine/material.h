#ifndef MATERIAL_H_
#define MATERIAL_H_

struct Material
{
  Material();
  Material(string &file);
  ~Material();
  //void LoadMaterial(string &material);
  void SetupUniforms();

  string name_;
  int shader_id_;
  ShaderUniforms *shader_uniforms_;
  vec3 ambient_;
  vec3 diffuse_;
  vec3 specular_;
  float shininess_;
  float alpha_;
  GLuint has_diff_tex;
  GLuint has_norm_tex;

  GLuint tex_diff_;
  GLuint tex_bump_;
  GLuint tex_spec_;
};

#endif