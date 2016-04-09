#ifndef UTILITY_H_
#define UTILITY_H_

namespace Utility {
  GLuint LoadShaders(string vs, string fs, string gs);
  vector<Material*> LoadMaterial(string &file);
  vector<Object*> LoadModels(string &file, vector<Material*> &materials);
  void LoadDDSTexture(GLuint &texture, const char *texture_file);
}

#endif
