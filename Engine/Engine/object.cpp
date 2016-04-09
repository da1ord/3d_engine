#include "main.h"

Object::Object() {
}

Object::~Object() {
  //TODO: foreach model -> call destroy + delete
}

void Object::LoadModels(string &file) {
  //TODO: to utility
  string line;
  int line_count = 0;
  int i = 0;
  int v = 0;
  int vt = 0;
  int vn = 0;
  int f = 0;
  int offset = 0;
  
  Model *m = NULL;
  char *buffer;
  string contents;

  ifstream model(file);
  if (model) {
    model.seekg(0, ios::end);
    contents.resize(model.tellg());
    model.seekg(0, ios::beg);
    model.read(&contents[0], contents.size());
    model.close();
  }
  int f1 = 0;
  int f2 = 0;
  string s;
  while (f1 < contents.size()) {
    if (f1 == 0) {
      f1 = -1;
    }
    f2 = contents.find('\n', f1 + 1);
    
    s = contents.substr(f1 + 1, f2 - f1 - 1);
    if (s.length() > 0) {
      if (s.substr(0, 2) == "o ") {
        Object *o = new Object();
      } 
      if (s.substr(0, 6) == "usemtl") {
        cout << s << endl;
        if (m != NULL) {
          offset = v;
          //m->shader_id_ = Utility::LoadShaders(vs, fs, gs);
          //m->SetupUniforms();
          models_.push_back(m);
        }
        m = new Model();
        m->name_ = s.substr(2, s.length() - 2);
        v = 0;
        vt = 0;
        vn = 0;
        f = 0;

        //sscanf_s(line.c_str(), "%*s %*s %d", &num_verts_);
        //vertices_ = new Vertex[num_verts_];
      }
      else if (s.substr(0, 2) == "v " ) {
        v++;
      }
      else if (s.substr(0, 2) == "vt" ) {
        vt++;
      }
      else if (s.substr(0, 2) == "vn" ) {
        vn++;
      }
      else if (s.substr(0, 2) == "f " ) {
        f++;
      }
    }

    f1 = f2;
  }


  if (model.is_open()) {
    while (getline(model, line)) {
      if (line.length() > 0) {
        if (line.substr(0, 2) == "o ") {
          if (m != NULL) {
            //m->shader_id_ = Utility::LoadShaders(vs, fs, gs);
            //m->SetupUniforms();
            models_.push_back(m);
          }
          m = new Model();
          m->name_ = line.substr(2, line.length() - 2);
          v = 0;
          vt = 0;
          vn = 0;
          //sscanf_s(line.c_str(), "%*s %*s %d", &num_verts_);
          //vertices_ = new Vertex[num_verts_];
        }
        else if (line.substr(0, 2) == "v " ) {
          v++;
        }
        else if (line.substr(0, 2) == "vt" ) {
          vt++;
        }
        else if (line.substr(0, 2) == "vn" ) {
          vn++;
        }
        //else if (line.find("element face ") != std::string::npos) {
        //  sscanf_s(line.c_str(), "%*s %*s %d", &num_faces_);
        //  num_indices_ = num_faces_ * 3;
        //  indices_ = new GLuint[num_indices_];
        //}
        //else if (line.substr(0, 2) == "3 " && line_count > num_verts_ + 15) {
        //  int face[3];
        //  sscanf_s(line.c_str(), "%*d %d %d %d", &face[0], &face[1], &face[2]);

        //  indices_[i++] = face[0];
        //  indices_[i++] = face[1];
        //  indices_[i++] = face[2];
        //  
        //  Face *fa = new Face();
        //  fa->indices_[0] = face[0];
        //  fa->indices_[1] = face[1];
        //  fa->indices_[2] = face[2];
        //  fa->verts_[0] = &vertices_[face[0]];
        //  fa->verts_[1] = &vertices_[face[1]];
        //  fa->verts_[2] = &vertices_[face[2]];
        //  //fa->material_ = &geometry.material_;
        //  fa->center_.x = fa->verts_[0]->position_.x + fa->verts_[1]->position_.x + fa->verts_[2]->position_.x;
        //  fa->center_.x /= 3.0;
        //  fa->center_.y = fa->verts_[0]->position_.y + fa->verts_[1]->position_.y + fa->verts_[2]->position_.y;
        //  fa->center_.y /= 3.0;
        //  fa->center_.z = fa->verts_[0]->position_.z + fa->verts_[1]->position_.z + fa->verts_[2]->position_.z;
        //  fa->center_.z /= 3.0;
        //  faces_.push_back(fa);
        //}
        //else {
        //  if (line_count > 15) {
        //    sscanf_s(line.c_str(), "%f %f %f %f %f %f %f %f", 
        //      &vertices_[v].position_.x, &vertices_[v].position_.y,
        //      &vertices_[v].position_.z, &vertices_[v].normal_.x, 
        //      &vertices_[v].normal_.y, &vertices_[v].normal_.z, 
        //      &vertices_[v].tex_uv_.x, &vertices_[v].tex_uv_.y);
      
        //    if (vertices_[v].position_.x < aabb->bounds_[0].x) {
        //      aabb->bounds_[0].x = vertices_[v].position_.x;
        //    }
        //    if (vertices_[v].position_.x > aabb->bounds_[1].x) {
        //      aabb->bounds_[1].x = vertices_[v].position_.x;
        //    }
        //    if (vertices_[v].position_.y < aabb->bounds_[0].y) {
        //      aabb->bounds_[0].y = vertices_[v].position_.y;
        //    }
        //    if (vertices_[v].position_.y > aabb->bounds_[1].y) {
        //      aabb->bounds_[1].y = vertices_[v].position_.y;
        //    }
        //    if (vertices_[v].position_.z < aabb->bounds_[0].z) {
        //      aabb->bounds_[0].z = vertices_[v].position_.z;
        //    }
        //    if (vertices_[v].position_.z > aabb->bounds_[1].z) {
        //      aabb->bounds_[1].z = vertices_[v].position_.z;
        //    }
        //    v++;
        //  }
        //}
      }
    }
    model.close();
  }
}