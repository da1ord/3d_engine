#include "main.h"

GLuint Utility::LoadShaders(string vs, string fs, string gs) {                                                                                       
  GLuint shader_id = glCreateProgram();

  // Create the shaders
  GLuint vs_id, fs_id, gs_id;
  
  string line = "";
 
  GLint result = GL_FALSE;
  int info_log_length;
 
  if (vs != "") {
    vs_id = glCreateShader(GL_VERTEX_SHADER);
    string vs_code;
    ifstream vs_stream(vs);
    if (vs_stream.is_open()) {
      while (getline(vs_stream, line))
        vs_code += "\n" + line;
      vs_stream.close();
    }

    // Compile Vertex Shader
    cout << "Compiling shader: " << vs << endl;
    char const* vs_pointer = vs_code.c_str();
    glShaderSource(vs_id, 1, &vs_pointer , NULL);
    glCompileShader(vs_id);
 
    // Check Vertex Shader
    glGetShaderiv(vs_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vs_id, GL_INFO_LOG_LENGTH, &info_log_length);
    info_log_length = 1024;
    vector<char> vs_error(info_log_length);
    glGetShaderInfoLog(vs_id, info_log_length, NULL,
      &vs_error[0]);
    if (vs_error[0] != '\0') {
      cout << &vs_error[0] << endl;
    }
    glAttachShader(shader_id, vs_id);
  }
  if (fs != "") {
    fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    string fs_code;
    ifstream fs_stream(fs);
    if (fs_stream.is_open()) {
      while (getline(fs_stream, line))
        fs_code += "\n" + line;
      fs_stream.close();
    }

    // Compile Fragment Shader
    cout << "Compiling shader: " << fs << endl;
    char const* fs_pointer = fs_code.c_str();
    glShaderSource(fs_id, 1, &fs_pointer , NULL);
    glCompileShader(fs_id);
 
    // Check Fragment Shader
    glGetShaderiv(fs_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fs_id, GL_INFO_LOG_LENGTH, &info_log_length);
    info_log_length = 1024;
    vector<char> fs_error(info_log_length);
    glGetShaderInfoLog(fs_id, info_log_length, NULL,
      &fs_error[0]);
	if (fs_error[0] != '\0') {
      cout << &fs_error[0] << endl;
    }
    glAttachShader(shader_id, fs_id);
  }
  if (gs != "") {
    gs_id = glCreateShader(GL_GEOMETRY_SHADER);
    string gs_code;
    ifstream gs_stream(gs);
    if (gs_stream.is_open()) {
      while (getline(gs_stream, line))
        gs_code += "\n" + line;
      gs_stream.close();
    }

    // Compile Fragment Shader
    cout << "Compiling shader: " << gs << endl;
    char const* gs_pointer = gs_code.c_str();
    glShaderSource(gs_id, 1, &gs_pointer , NULL);
    glCompileShader(gs_id);
 
    // Check Fragment Shader
    glGetShaderiv(gs_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(gs_id, GL_INFO_LOG_LENGTH, &info_log_length);
	info_log_length = 1024;
    vector<char> gs_error(info_log_length);
    glGetShaderInfoLog(gs_id, info_log_length, NULL,
      &gs_error[0]);
	if (gs_error[0] != '\0') {
      cout << &gs_error[0] << endl;
    }
    glAttachShader(shader_id, gs_id);
  }

  // Link the program
  cout << "Linking program" << endl;
  glLinkProgram(shader_id);
  
  glBindAttribLocation(shader_id, 0, "inPosition");
  glBindAttribLocation(shader_id, 1, "inNormal");
  glBindAttribLocation(shader_id, 2, "inTexCoord");
  glBindAttribLocation(shader_id, 3, "inTangent");
  glBindAttribLocation(shader_id, 4, "inBinormal");
 
  // Check the program
  glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
  glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  vector<char> p_error(glm::max(info_log_length, int(1)));
  glGetProgramInfoLog(shader_id, info_log_length, NULL, &p_error[0]);
  if (p_error.size() > 1) {
    cout << &p_error[0] << endl;
  }
  
  if (vs != "") {
    glDeleteShader(vs_id);
  }
  if (fs != "") {
    glDeleteShader(fs_id);
  }
  if (gs != "") {
    glDeleteShader(gs_id);
  }

  cout << "Shaders loaded.\n" << endl;

  return shader_id;
}

vector<Material*> Utility::LoadMaterial(string &file) {  
  vector<Material*> materials;
  string vs, fs, gs;

  Material *m = NULL;
  string line;
  ifstream material(file);
  if (material.is_open()) {
    while (getline(material, line)) {
      if (line.substr(0, 1) == "#") {
        continue;
      }
      else if (line.find("newmtl ") != std::string::npos) {
        if (m != NULL) {
          //TODO: load shaders once
          m->shader_id_ = Utility::LoadShaders(vs, fs, gs);
          m->SetupUniforms();
          materials.push_back(m);
        }
        m = new Material();
        m->shader_uniforms_ = new ShaderUniforms();
        m->name_ = line.substr(7, line.length() - 7);
        //sscanf_s(line.c_str(), "%*s %s", &m->name_[0]);
      }
      else if (line.find("map_Kd ") != std::string::npos) {
        string diff_tex;
        diff_tex = line.substr(7, diff_tex.length() - 7);
		cout << "Loading diffuse texture '" << diff_tex << "'." << endl;
        Utility::LoadDDSTexture(m->tex_diff_, diff_tex.c_str());
		cout << "Diffuse texture '" << diff_tex << "' loaded." << endl;
        m->has_diff_tex = 1;
      }
      else if (line.find("Ka ") != std::string::npos) {
        sscanf_s(line.c_str(), "%*s %f %f %f", &m->ambient_.x, &m->ambient_.y, 
          &m->ambient_.z);
      }
      else if (line.find("Kd ") != std::string::npos) {
        sscanf_s(line.c_str(), "%*s %f %f %f", &m->diffuse_.x, &m->diffuse_.y, 
          &m->diffuse_.z);
      }
      else if (line.find("Ks ") != std::string::npos) {
        sscanf_s(line.c_str(), "%*s %f %f %f", &m->specular_.x, 
          &m->specular_.y, &m->specular_.z);
      }
      else if (line.find("Ns ") != std::string::npos) {
        sscanf_s(line.c_str(), "%*s %f", &m->shininess_);
      }
      else if (line.find("alpha ") != std::string::npos) {
        sscanf_s(line.c_str(), "%*s %f", &m->alpha_);
      }
      else if (line.find("vert ") != std::string::npos) {
        vs = line.substr(5, line.length() - 5);
        //sscanf_s(line.c_str(), "%*s %s", &vs);
      }
      else if (line.find("frag ") != std::string::npos) {
        fs = line.substr(5, line.length() - 5);
        //sscanf_s(line.c_str(), "%*s %s", &fs[0]);
      }
      else if (line.find("geom ") != std::string::npos) {
        gs = line.substr(5, line.length() - 5);
        //sscanf_s(line.c_str(), "%*s %s", &gs[0]);
      }
    }
  }
  material.close();
  if (m != NULL) {
    m->shader_id_ = Utility::LoadShaders(vs, fs, gs);
    m->SetupUniforms();
    materials.push_back(m);
  }

  return materials;
}

vector<Object*> Utility::LoadModels(string &file, 
    vector<Material*> &materials) {
  string line;
  int line_count = 0;
  int i = 0;
  int v = 0;
  int vt = 0;
  int vn = 0;
  int offset_v = 0;
  int offset_vt = 0;
  int offset_vn = 0;
  
  int total_verts = 0;
  int total_faces = 0;

  Model *m = NULL;
  string contents;
  vector<Object*> objects;
  vector<vec3> positions;
  vector<vec2> tex_coords;
  vector<vec3> normals;
  vector<Vertex> verts;
  vector<GLuint> indices;
  Object *o = NULL;
  AABB *aabb;

  int slash_pos = file.find_last_of("/");
  string file_path;
  if (slash_pos >= 0) {
    file_path = file.substr(0, slash_pos + 1);
  }
  
  cout << "========================================"  << endl 
      << "Loading model '" << file << "'." << endl;
  ifstream model(file);
  if (model) {
    model.seekg(0, ios::end);
    contents.resize(static_cast<uint>(model.tellg()));
    model.seekg(0, ios::beg);
    model.read(&contents[0], contents.size());
    model.close();
  }
  uint f1 = 0;
  uint f2 = 0;
  string s;

  while (f1 < contents.size()) {
    if (f1 == 0) {
      f1 = -1;
    }
    f2 = contents.find('\n', f1 + 1);
    
    s = contents.substr(f1 + 1, f2 - f1 - 1);
    if (s.length() > 0) {
      if (s.substr(0, 2) == "o ") {
        if (o != NULL) {
          if (m != NULL) {
            m->geometry_[0]->vertices_ = &verts[0];
            m->geometry_[0]->indices_ = &indices[0];
            m->geometry_[0]->num_verts_ = verts.size();
            m->geometry_[0]->num_indices_ = indices.size();

            m->geometry_[0]->CreateBufferObjects();
            aabb->Recalculate();
            m->bounding_box_ = aabb;
            o->models_.push_back(m);

            m = NULL;
          }
          offset_v += v;
          offset_vt += vt;
          offset_vn += vn;
          objects.push_back(o);
          verts.clear();
          indices.clear();
          positions.clear();
          tex_coords.clear();
          normals.clear();
        }
        v = 0;
        vt = 0;
        vn = 0;
        
        o = new Object();
        aabb = new AABB();
        o->name_ = s.substr(2, s.length() - 2);
      }
      else if (s.substr(0, 6) == "mtllib") {
        string material_file = file_path + s.substr(7, s.length() - 7);
        vector<Material*> mats = Utility::LoadMaterial(material_file);
        materials.insert(materials.end(), mats.begin(), mats.end());
      }
      else if (s.substr(0, 6) == "usemtl") {
        if (m != NULL) {
          m->geometry_[0]->vertices_ = &verts[0];
          m->geometry_[0]->indices_ = &indices[0];
          m->geometry_[0]->num_verts_ = verts.size();
          m->geometry_[0]->num_indices_ = indices.size();
          m->geometry_[0]->CreateBufferObjects();
          aabb->Recalculate();
          m->bounding_box_ = aabb;
          if (o != NULL) {
            o->models_.push_back(m);
          }
          verts.clear();
          indices.clear();
        }
        m = new Model();
        
        m->name_ = s.substr(7, s.length() - 7);
        cout << o->name_ << " setting material ... ";
        for (unsigned int i = 0; i < materials.size(); i++) {
          if (m->name_ == materials[i]->name_) {
            m->material_ = materials[i];
            cout << m->name_;
          }
        }
        cout << endl;
        m->geometry_.push_back(new Geometry());
      }
      else if (s.substr(0, 2) == "v " ) {
        v++;
        vec3 pos;
        sscanf_s(s.c_str(), "%*s %f %f %f", &pos.x, &pos.y, &pos.z);
        positions.push_back(pos);
        aabb->UpdateBounds(pos);
      }
      else if (s.substr(0, 2) == "vt" ) {
        vt++;
        vec2 tex;
        sscanf_s(s.c_str(), "%*s %f %f", &tex.x, &tex.y);
        tex_coords.push_back(tex);
      }
      else if (s.substr(0, 2) == "vn" ) {
        vn++;
        vec3 norm;
        sscanf_s(s.c_str(), "%*s %f %f %f", &norm.x, &norm.y, &norm.z);
        normals.push_back(norm);
      }
      else if (s.substr(0, 2) == "f " ) {
        if (vt > 0 && vn > 0) {
          vec3 pos, tex, norm;
          sscanf_s(s.c_str(), "%*s %f/%f/%f %f/%f/%f %f/%f/%f", &pos.x, &tex.x, &norm.x, &pos.y, &tex.y, &norm.y, &pos.z, &tex.z, &norm.z);
          Vertex vert;
          vert.position_ = positions[(uint)pos.x - offset_v - 1];
          vert.tex_uv_ = tex_coords[(uint)tex.x - offset_vt - 1];
          vert.normal_ = normals[(uint)norm.x - offset_vn - 1];
          verts.push_back(vert);
          indices.push_back(verts.size() - 1);
          vert.position_ = positions[(uint)pos.y - offset_v - 1];
          vert.tex_uv_ = tex_coords[(uint)tex.y - offset_vt - 1];
          vert.normal_ = normals[(uint)norm.y - offset_vn - 1];
          verts.push_back(vert);
          indices.push_back(verts.size() - 1);
          vert.position_ = positions[(uint)pos.z - offset_v - 1];
          vert.tex_uv_ = tex_coords[(uint)tex.z - offset_vt - 1];
          vert.normal_ = normals[(uint)norm.z - offset_vn - 1];
          verts.push_back(vert);
          indices.push_back(verts.size() - 1);
        }
        total_faces++;
      }
    }

    f1 = f2;
  }
  total_verts = offset_v + v;
  cout << "Model '" << file << "' loaded (vertices: " << total_verts << 
    ", faces: " << total_faces << ")." << endl;

  if (m != NULL) {
    m->geometry_[0]->vertices_ = &verts[0];
    m->geometry_[0]->indices_ = &indices[0];

    m->geometry_[0]->num_verts_ = verts.size();
    m->geometry_[0]->num_indices_ = indices.size();
    m->geometry_[0]->CreateBufferObjects();
    aabb->Recalculate();
    m->bounding_box_ = aabb;
    if (o != NULL) {
      o->models_.push_back(m);
      objects.push_back(o);
    }
  }
  verts.clear();
  indices.clear();
  positions.clear();
  tex_coords.clear();
  normals.clear();

  return objects;
}

void Utility::LoadDDSTexture(GLuint &texture, const char *texture_file) {
  storage storage(load_dds(texture_file));
  assert(!storage.empty());

  GLenum target = storage.layers() > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
  glGenTextures(1, &texture);
  glBindTexture(target, texture);
  glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 
    static_cast<GLint>(storage.levels() - 1));
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, 
    storage.levels() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if (storage.layers() > 1) {
    texture2DArray texture(storage);
    glTexStorage3D(target, static_cast<GLint>(texture.levels()),
    static_cast<GLenum>(internal_format(texture.format())),
    static_cast<GLsizei>(texture.dimensions().x),
    static_cast<GLsizei>(texture.dimensions().y),
    static_cast<GLsizei>(texture.layers()));
    if (is_compressed(texture.format())) {
      for (texture2DArray::size_type layer = 0; layer < texture.layers(); 
          ++layer) {
        for (texture2DArray::size_type level = 0; level < texture.levels(); 
            ++level) {
          glCompressedTexSubImage3D(target,
          static_cast<GLint>(level),
          0, 0, static_cast<GLint>(layer),
          static_cast<GLsizei>(texture[layer][level].dimensions().x),
          static_cast<GLsizei>(texture[layer][level].dimensions().y),
          static_cast<GLsizei>(1),
          static_cast<GLenum>(external_format(texture.format())),
          static_cast<GLsizei>(texture[layer][level].size()),
          texture[layer][level].data());
        }
      }
    }
    else {
      for (texture2DArray::size_type layer = 0; layer < texture.layers(); 
          ++layer) {
        for (texture2DArray::size_type level = 0; level < texture.levels(); 
            ++level) {
          glTexSubImage3D(target,
          static_cast<GLint>(level),
          0, 0, static_cast<GLint>(layer),
          static_cast<GLsizei>(texture[layer][level].dimensions().x),
          static_cast<GLsizei>(texture[layer][level].dimensions().y),
          static_cast<GLsizei>(1),
          static_cast<GLenum>(external_format(texture.format())),
          static_cast<GLenum>(type_format(texture.format())),
          texture[layer][level].data());
        }
      }
    }
  }
  else {
    texture2D texture(storage);
    texture = flip(texture);
    glTexStorage2D(target, static_cast<GLint>(texture.levels()),
    static_cast<GLenum>(internal_format(texture.format())),
    static_cast<GLsizei>(texture.dimensions().x), 
    static_cast<GLsizei>(texture.dimensions().y));
    if (is_compressed(texture.format())) {
      for (texture2D::size_type level = 0; level < texture.levels(); ++level) {
        glCompressedTexSubImage2D(target,
        static_cast<GLint>(level), 0, 0,
        static_cast<GLsizei>(texture[level].dimensions().x),
        static_cast<GLsizei>(texture[level].dimensions().y),
        static_cast<GLenum>(external_format(texture.format())),
        static_cast<GLsizei>(texture[level].size()),
        texture[level].data());
       }
    }
    else {
      for (texture2D::size_type level = 0; level < texture.levels(); ++level) {
        glTexSubImage2D(target,
        static_cast<GLint>(level), 0, 0,
        static_cast<GLsizei>(texture[level].dimensions().x),
        static_cast<GLsizei>(texture[level].dimensions().y),
        GL_RGBA,//static_cast<GLenum>(external_format(texture.format())),
        static_cast<GLenum>(type_format(texture.format())),
        texture[level].data());
      }
    }
  }
  glBindTexture(target, 0);
}
