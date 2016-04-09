#ifndef MODEL_H_
#define MODEL_H_

class Model {
 public:
  Model();
  Model(string &model);
  ~Model();
  void LoadGeometry(string &model);
  void LoadGeometryOBJ(string &model);
  void BindUniforms(mat4 &mvp);
  void BindUniforms(mat4 &mvp, mat4 &v, GLuint &shadowTex, 
    mat4 *shadowMats, float *farPlanes);
  void Draw(int lod);
  void DrawFSQuadTex(GLuint &tex, float level, float scale, vec2 offset);
  void DrawFSQuadTex(GLuint &tex, float scale, vec2 offset);
  bool IsSameShader(int shader_id);

  string name_;
  PxRigidActor *nxActor_;
  vector<Geometry*> geometry_;
  AABB *bounding_box_;
  mat4 model_matrix_;
  Material *material_;
  GLuint tex_diff_;
  GLuint tex_bump_;
  GLuint tex_spec_;
  int lod_levels_;
  int lod_;
};

#endif