#include "main.h"

Model::Model() {
  bounding_box_ = new AABB();

  lod_levels_ = 0;
  lod_ = 0;
}

Model::Model(string &model) {
  bounding_box_ = new AABB();
  LoadGeometry(model);
  
  lod_levels_ = 0;
  lod_ = 0;
}

Model::~Model() {
}

void Model::LoadGeometry(string &model) {
  // TODO: for every geometry LOD
  Geometry *g = new Geometry(model, bounding_box_);
  geometry_.push_back(g);
}

//void Model::LoadGeometryOBJ(string &model) {
//  //string line;
//  //int line_count = 0;
//  //int i = 0;
//  //int v = 0;
//  //
//  //ifstream geometry(model);
//  //if (geometry.is_open()) {
//  //  while (getline(geometry, line)) {
//  //    if (line.length() > 0) {
//  //      if (line.find("o ") != std::string::npos) {
//  //        sscanf_s(line.c_str(), "%*s %*s %d", &num_verts_);
//  //        vertices_ = new Vertex[num_verts_];
//  //      }
//  //      else if (line.find("element face ") != std::string::npos) {
//  //        sscanf_s(line.c_str(), "%*s %*s %d", &num_faces_);
//  //        num_indices_ = num_faces_ * 3;
//  //        indices_ = new GLuint[num_indices_];
//  //      }
//  //      else if (line.substr(0, 2) == "3 " && line_count > num_verts_ + 15) {
//  //        int face[3];
//  //        sscanf_s(line.c_str(), "%*d %d %d %d", &face[0], &face[1], &face[2]);
//
//  //        indices_[i++] = face[0];
//  //        indices_[i++] = face[1];
//  //        indices_[i++] = face[2];
//  //        
//  //        Face *fa = new Face();
//  //        fa->indices_[0] = face[0];
//  //        fa->indices_[1] = face[1];
//  //        fa->indices_[2] = face[2];
//  //        fa->verts_[0] = &vertices_[face[0]];
//  //        fa->verts_[1] = &vertices_[face[1]];
//  //        fa->verts_[2] = &vertices_[face[2]];
//  //        //fa->material_ = &geometry.material_;
//  //        fa->center_.x = fa->verts_[0]->position_.x + fa->verts_[1]->position_.x + fa->verts_[2]->position_.x;
//  //        fa->center_.x /= 3.0;
//  //        fa->center_.y = fa->verts_[0]->position_.y + fa->verts_[1]->position_.y + fa->verts_[2]->position_.y;
//  //        fa->center_.y /= 3.0;
//  //        fa->center_.z = fa->verts_[0]->position_.z + fa->verts_[1]->position_.z + fa->verts_[2]->position_.z;
//  //        fa->center_.z /= 3.0;
//  //        faces_.push_back(fa);
//  //      }
//  //      else {
//  //        if (line_count > 15) {
//  //          sscanf_s(line.c_str(), "%f %f %f %f %f %f %f %f", 
//  //            &vertices_[v].position_.x, &vertices_[v].position_.y,
//  //            &vertices_[v].position_.z, &vertices_[v].normal_.x, 
//  //            &vertices_[v].normal_.y, &vertices_[v].normal_.z, 
//  //            &vertices_[v].tex_uv_.x, &vertices_[v].tex_uv_.y);
//  //    
//  //          if (vertices_[v].position_.x < aabb->bounds_[0].x) {
//  //            aabb->bounds_[0].x = vertices_[v].position_.x;
//  //          }
//  //          if (vertices_[v].position_.x > aabb->bounds_[1].x) {
//  //            aabb->bounds_[1].x = vertices_[v].position_.x;
//  //          }
//  //          if (vertices_[v].position_.y < aabb->bounds_[0].y) {
//  //            aabb->bounds_[0].y = vertices_[v].position_.y;
//  //          }
//  //          if (vertices_[v].position_.y > aabb->bounds_[1].y) {
//  //            aabb->bounds_[1].y = vertices_[v].position_.y;
//  //          }
//  //          if (vertices_[v].position_.z < aabb->bounds_[0].z) {
//  //            aabb->bounds_[0].z = vertices_[v].position_.z;
//  //          }
//  //          if (vertices_[v].position_.z > aabb->bounds_[1].z) {
//  //            aabb->bounds_[1].z = vertices_[v].position_.z;
//  //          }
//  //          v++;
//  //        }
//  //      }
//  //    }
//  //  }
//  //  geometry.close();
//  //}
//}

void Model::BindUniforms(mat4 &mvp) {
  glUniformMatrix4fv(material_->shader_uniforms_->MVP, 1, GL_FALSE, value_ptr(mvp));
}

void Model::BindUniforms(mat4 &mvp, mat4 &v, GLuint &shadowTex, mat4 *shadowMats, float *farPlanes) {
  // Activate and bind diffuse texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, material_->tex_diff_);
  glUniform1i(material_->shader_uniforms_->tex_diff, 0);

  // Activate and bind alpha texture
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, material_->tex_alpha_);
  glUniform1i(material_->shader_uniforms_->tex_alpha, 1);
  
  // Activate and bind static shadow texture
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTex);
  int shadow_map_loc = glGetUniformLocation(material_->shader_id_, "shadowMap");
  glUniform1i(shadow_map_loc, 2);

  // Matrices
  glUniformMatrix4fv(material_->shader_uniforms_->MVP, 1, GL_FALSE, value_ptr(mvp));
  glUniformMatrix4fv(material_->shader_uniforms_->V_inv, 1, GL_FALSE, value_ptr(inverse(v)));
  mat3 N = transpose(inverse(mat3(model_matrix_)));// * model_matrix_)); 
  glUniformMatrix3fv(material_->shader_uniforms_->N, 1, GL_FALSE, value_ptr(N));
  glUniformMatrix3fv(material_->shader_uniforms_->M, 1, GL_FALSE, value_ptr(model_matrix_));
  
  int shadow_mvp_loc = glGetUniformLocation(material_->shader_id_, "shadowMVP");
  glUniformMatrix4fv(shadow_mvp_loc, 3, GL_FALSE, value_ptr(shadowMats[0]));
  
  vec3 farPlanesVec = vec3(farPlanes[0], farPlanes[1], farPlanes[2]);
  glUniform3fv(glGetUniformLocation(material_->shader_id_, "farPlanes"), 1, value_ptr(farPlanesVec));

  // Material properties
  glUniform4fv(material_->shader_uniforms_->ambientColor, 1, value_ptr(material_->ambient_));
  glUniform4fv(material_->shader_uniforms_->diffuseColor, 1, value_ptr(material_->diffuse_));
  glUniform4fv(material_->shader_uniforms_->specularColor, 1, value_ptr(material_->specular_));
  glUniform1f(material_->shader_uniforms_->shininess, material_->shininess_);
  glUniform1f(material_->shader_uniforms_->alpha, material_->alpha_);

  //glBindFragDataLocation(material_->shader_id_, 0, "outColor");
  
  
  //glUniform1f(m->shader_uniforms_->time, time_);
  
  glUniform1f(material_->shader_uniforms_->has_diff, material_->has_diff_tex);
  glUniform1f(material_->shader_uniforms_->has_norm, material_->has_norm_tex);
  glUniform1f(material_->shader_uniforms_->has_alpha, material_->has_alpha_tex);
}

void Model::Draw(int lod) {
  // Bind and draw VAO
  glBindVertexArray(geometry_[lod]->vao_);
  if (name_ == "cube") {
    glDrawElementsInstanced(GL_TRIANGLES, geometry_[lod]->num_indices_, 
      GL_UNSIGNED_INT, NULL, 1);//4096
  }
  else {
    glDrawElements(GL_TRIANGLES, geometry_[lod]->num_indices_, GL_UNSIGNED_INT, NULL);
  }
  glBindVertexArray(0);
}

void Model::DrawFSQuadTex(GLuint &tex, float level, float scale, vec2 offset) {
  // Activate shader program
  glUseProgram(material_->shader_id_);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
  
  glUniform1f(glGetUniformLocation(material_->shader_id_, "scale"), scale);
  glUniform2fv(glGetUniformLocation(material_->shader_id_, "offset"), 1, value_ptr(offset));
  glUniform1i(material_->shader_uniforms_->tex_diff, 0);
  glUniform1f(glGetUniformLocation(material_->shader_id_, "level"), level);

  // Bind and draw VAO
  Draw(0);

  // Unbind and deactivate textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

  // Deactivate shader program
  glUseProgram(0);
}

void Model::DrawFSQuadTex(GLuint &tex, float scale, vec2 offset) {
  // Activate shader program
  glUseProgram(material_->shader_id_);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  glUniform1f(glGetUniformLocation(material_->shader_id_, "scale"), scale);
  glUniform2fv(glGetUniformLocation(material_->shader_id_, "offset"), 1, 
    value_ptr(offset));
  glUniform1i(material_->shader_uniforms_->tex_diff, 0);

  // Bind and draw VAO
  Draw(0);

  // Unbind and deactivate textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Deactivate shader program
  glUseProgram(0);
}

bool Model::IsSameShader(int shader_id) {
  return shader_id == material_->shader_id_;
}
