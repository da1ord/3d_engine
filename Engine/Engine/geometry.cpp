#include "main.h"

Geometry::Geometry() {
}

Geometry::Geometry(string &file, AABB *aabb) {
  LoadGeometry(file, aabb);
}

Geometry::~Geometry() {
}

void Geometry::LoadGeometry(string &file, AABB *aabb) {
  string line;
  int line_count = 0;
  int i = 0;
  int v = 0;
  
  ifstream geometry(file);
  if (geometry.is_open()) {
    while (getline(geometry, line)) {
      line_count++;
      if (line.length() > 0) {
        if (line.find("element vertex ") != std::string::npos) {
          sscanf_s(line.c_str(), "%*s %*s %d", &num_verts_);
          vertices_ = new Vertex[num_verts_];
        }
        else if (line.find("element face ") != std::string::npos) {
          sscanf_s(line.c_str(), "%*s %*s %d", &num_faces_);
          num_indices_ = num_faces_ * 3;
          indices_ = new GLuint[num_indices_];
        }
        else if (line.substr(0, 2) == "3 " && line_count > num_verts_ + 15) {
          int face[3];
          sscanf_s(line.c_str(), "%*d %d %d %d", &face[0], &face[1], &face[2]);

          indices_[i++] = face[0];
          indices_[i++] = face[1];
          indices_[i++] = face[2];
          
          Face *fa = new Face();
          fa->indices_[0] = face[0];
          fa->indices_[1] = face[1];
          fa->indices_[2] = face[2];
          fa->verts_[0] = &vertices_[face[0]];
          fa->verts_[1] = &vertices_[face[1]];
          fa->verts_[2] = &vertices_[face[2]];
          //fa->material_ = &geometry.material_;
          fa->center_.x = fa->verts_[0]->position_.x + fa->verts_[1]->position_.x + fa->verts_[2]->position_.x;
          fa->center_.x /= 3.0;
          fa->center_.y = fa->verts_[0]->position_.y + fa->verts_[1]->position_.y + fa->verts_[2]->position_.y;
          fa->center_.y /= 3.0;
          fa->center_.z = fa->verts_[0]->position_.z + fa->verts_[1]->position_.z + fa->verts_[2]->position_.z;
          fa->center_.z /= 3.0;
          faces_.push_back(fa);
        }
        else {
          if (line_count > 15) {
            sscanf_s(line.c_str(), "%f %f %f %f %f %f %f %f", 
              &vertices_[v].position_.x, &vertices_[v].position_.y,
              &vertices_[v].position_.z, &vertices_[v].normal_.x, 
              &vertices_[v].normal_.y, &vertices_[v].normal_.z, 
              &vertices_[v].tex_uv_.x, &vertices_[v].tex_uv_.y);
      
            if (vertices_[v].position_.x < aabb->bounds_[0].x) {
              aabb->bounds_[0].x = vertices_[v].position_.x;
            }
            if (vertices_[v].position_.x > aabb->bounds_[1].x) {
              aabb->bounds_[1].x = vertices_[v].position_.x;
            }
            if (vertices_[v].position_.y < aabb->bounds_[0].y) {
              aabb->bounds_[0].y = vertices_[v].position_.y;
            }
            if (vertices_[v].position_.y > aabb->bounds_[1].y) {
              aabb->bounds_[1].y = vertices_[v].position_.y;
            }
            if (vertices_[v].position_.z < aabb->bounds_[0].z) {
              aabb->bounds_[0].z = vertices_[v].position_.z;
            }
            if (vertices_[v].position_.z > aabb->bounds_[1].z) {
              aabb->bounds_[1].z = vertices_[v].position_.z;
            }
            v++;
          }
        }
      }
    }
    geometry.close();
  }

  for (int i = 0; i < num_faces_; i++) {
    int vert_indices[3] = { indices_[i * 3 + 0], 
        indices_[i * 3 + 1], indices_[i * 3 + 2] };
    float v3v1, v2v1, u3u1, u2u1;
    vec3 p3p1, p2p1;
    
    v3v1 = vertices_[vert_indices[2]].tex_uv_.y - 
        vertices_[vert_indices[0]].tex_uv_.y;
    v2v1 = vertices_[vert_indices[1]].tex_uv_.y - 
        vertices_[vert_indices[0]].tex_uv_.y;
    
    u3u1 = vertices_[vert_indices[2]].tex_uv_.x - 
        vertices_[vert_indices[0]].tex_uv_.x;
    u2u1 = vertices_[vert_indices[1]].tex_uv_.x - 
        vertices_[vert_indices[0]].tex_uv_.x;
    
    p3p1 = vertices_[vert_indices[2]].position_ - 
        vertices_[vert_indices[0]].position_;
    p2p1 = vertices_[vert_indices[1]].position_ - 
        vertices_[vert_indices[0]].position_;

    float t = u2u1 * v3v1 - v2v1 * u3u1;
    vec3 tangent;
    vec3 binormal;
    if (t == 0) {
      tangent = vec3(0, 0, 0);
    }
    else {
      tangent = (v3v1 * p2p1 - v2v1 * p3p1) / t;
    }
    t = v2v1 * u3u1 - u2u1 * v3v1;
    if (t == 0) {
      binormal = vec3(0, 0, 0);
    }
    else {
      binormal = (u3u1 * p2p1 - u2u1 * p3p1) / t;
    }
    
    vertices_[vert_indices[0]].tangent_ += tangent;
    vertices_[vert_indices[0]].binormal_ += binormal;
    vertices_[vert_indices[1]].tangent_ += tangent;
    vertices_[vert_indices[1]].binormal_ += binormal;
    vertices_[vert_indices[2]].tangent_ += tangent;
    vertices_[vert_indices[2]].binormal_ += binormal;
  }

  for (unsigned int v = 0; v < num_verts_; v++) {
    glm::normalize(vertices_[v].tangent_);
    glm::normalize(vertices_[v].binormal_);
  }

  cout << "Geometry loaded: " << num_verts_ << " vertices, " 
    << num_faces_ << " faces." << endl;

  CreateBufferObjects();
}

void Geometry::CreateBufferObjects() {
  cout << "Generating VAO, VBO, IBO ..." << endl;

  // Generate & bind VAO, VBO, IBO
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

  // Move data to GPU
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_verts_, vertices_, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices_, indices_, GL_STATIC_DRAW);


  // Set pointers to TexCoord, Normal and Vertex data
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    sizeof(Vertex),   // stride
    (void*)0            // array buffer offset
  );
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_TRUE,            // normalized?
    sizeof(Vertex),   // stride
    (void*)(sizeof(GLfloat) * 3)  // array buffer offset
  );
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
    2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    2,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    sizeof(Vertex),   // stride
    (void*)(sizeof(GLfloat) * 6)  // array buffer offset
  );
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(
    3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_TRUE,           // normalized?
    sizeof(Vertex),   // stride
    (void*)(sizeof(GLfloat) * 8)  // array buffer offset
  );
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(
    4,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_TRUE,           // normalized?
    sizeof(Vertex),   // stride
    (void*)(sizeof(GLfloat) * 11)  // array buffer offset
  );

  // Disable VAO
  glBindVertexArray(0);
  cout << "Deleting indices and vertices ..." << endl;
  
  //_CrtCheckMemory();

  // Delete vertices from memory, not necessary anymore
  //if (GL_STATIC_DRAW) {
    //if (indices_ != NULL) {
    //  delete[] indices_;
    //  indices_ = NULL;
    //}
    //if (vertices_ != NULL) {
    //  delete[] vertices_;
    //  vertices_ = NULL;
    //}
  //}

  cout << "VAO created." << endl;
}