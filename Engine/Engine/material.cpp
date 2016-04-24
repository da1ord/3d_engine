#include "main.h"

Material::Material() {
  has_diff_tex = 0;
  has_norm_tex = 0;
  has_alpha_tex = 0;

  alpha_ = 1.0f;
}

Material::Material(string &file) {
  has_diff_tex = 0;
  has_norm_tex = 0;
  has_alpha_tex = 0;

  alpha_ = 1.0f;
  //LoadMaterial(file);
}

Material::~Material() {
}

//void Material::LoadMaterial(string &file) {
//  //Texture *texture = new Texture();
//  //textures_.push_back(texture);
//  shader_uniforms_ = new ShaderUniforms();
//  
//  string vs, fs, gs;
//
//  string line;
//  ifstream material(file);
//  if (material.is_open()) {
//    while (getline(material, line)) {
//      if (line.find("newmtl ") != std::string::npos) {
//        name_ = line.substr(7, line.length() - 7);
//        //sscanf_s(line.c_str(), "%*s %s", &name_[0]);
//      }
//      else if (line.find("Ka ") != std::string::npos) {
//        sscanf_s(line.c_str(), "%*s %f %f %f", &ambient_.x, &ambient_.y, 
//          &ambient_.z);
//      }
//      else if (line.find("Kd ") != std::string::npos) {
//        sscanf_s(line.c_str(), "%*s %f %f %f", &diffuse_.x, &diffuse_.y, 
//          &diffuse_.z);
//      }
//      else if (line.find("Ks ") != std::string::npos) {
//        sscanf_s(line.c_str(), "%*s %f %f %f", &specular_.x, &specular_.y, 
//          &specular_.z);
//      }
//      else if (line.find("Ns ") != std::string::npos) {
//        sscanf_s(line.c_str(), "%*s %f", &shininess_);
//      }
//      else if (line.find("alpha ") != std::string::npos) {
//        sscanf_s(line.c_str(), "%*s %f", &alpha_);
//      }      
//      else if (line.find("map_Kd ") != std::string::npos) {
//        string diff_tex;
//        sscanf_s(line.c_str(), "%*s %s", &diff_tex[0]);
//        Utility::LoadDDSTexture(tex_diff_, diff_tex.c_str());
//        has_diff_tex = 1;
//      }
//      //else if (line.find("bump ") != std::string::npos) {
//      //  sscanf_s(line.c_str(), "%*s %f", alpha_);
//      //}
//      else if (line.find("vert ") != std::string::npos) {
//        vs = line.substr(5, line.length() - 5);
//        //sscanf_s(line.c_str(), "%*s %s", &vs);
//      }
//      else if (line.find("frag ") != std::string::npos) {
//        fs = line.substr(5, line.length() - 5);
//        //sscanf_s(line.c_str(), "%*s %s", &fs[0]);
//      }
//      else if (line.find("geom ") != std::string::npos) {
//        gs = line.substr(5, line.length() - 5);
//        //sscanf_s(line.c_str(), "%*s %s", &gs[0]);
//      }
//    }
//  }
//  material.close();
//
//  shader_id_ = Utility::LoadShaders(vs, fs, gs);
//  SetupUniforms();
//}

void Material::SetupUniforms() {
  shader_uniforms_->tex_diff = glGetUniformLocation(shader_id_, "tex_diff");
  shader_uniforms_->tex_norm = glGetUniformLocation(shader_id_, "tex_norm");
  shader_uniforms_->tex_alpha = glGetUniformLocation(shader_id_, "tex_alpha");
  shader_uniforms_->MVP = glGetUniformLocation(shader_id_, "MVP");
  shader_uniforms_->M = glGetUniformLocation(shader_id_, "M");
  shader_uniforms_->V_inv = glGetUniformLocation(shader_id_, "V_inv");
  shader_uniforms_->N = glGetUniformLocation(shader_id_, "N");
  shader_uniforms_->ambientColor = glGetUniformLocation(shader_id_, "ambientColor");
  shader_uniforms_->diffuseColor = glGetUniformLocation(shader_id_, "diffuseColor");
  shader_uniforms_->specularColor = glGetUniformLocation(shader_id_, "specularColor");
  shader_uniforms_->shininess = glGetUniformLocation(shader_id_, "shininess");
  shader_uniforms_->alpha = glGetUniformLocation(shader_id_, "alpha");
  shader_uniforms_->time = glGetUniformLocation(shader_id_, "time");
  shader_uniforms_->has_diff = glGetUniformLocation(shader_id_, "has_diff_tex");
  shader_uniforms_->has_norm = glGetUniformLocation(shader_id_, "has_norm_tex");
  shader_uniforms_->has_alpha = glGetUniformLocation(shader_id_, "has_alpha_tex");
}