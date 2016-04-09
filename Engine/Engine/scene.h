#ifndef SCENE_H_
#define SCENE_H_

class Scene
{
 public:
  Scene();
  Scene(SceneSettings *scene_settings);
  ~Scene();
  
  void Init();
  void InitOGL();
  void InitPX();
  
  //static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, 
  //  int mods);
  void KeyboardCallback();
  void MouseMoveCallback();
  void MouseClickCallback();
  void MouseScrollCallback(); 

  void LoadLevel(string &level_file);
  void LoadMaterials(string &level_file);
  void Run();

  void CalcShadowVPMats(float *farPlanes, mat4 *shadowVPMat, float weight);
  void CalcShadowMatrices(int csmLevel, float nearPlane, float farPlane);
  void GenerateShadowMap(mat4 &shadowVP, int level);

  GLFWwindow *window_;
  SceneSettings *scene_settings_;
  vector<Material*> materials_;
  vector<ShaderUniforms*> shader_uniforms_;
  
  vector<Object*> static_objects_;
  vector<Object*> dynamic_objects_;
  Object *terrain_;
  Object *fs_quad_;
  Object *fs_quad2_;
  SceneGraph *scene_graph_;
  //TerrainGraph *terrain_graph_;

  ShadowMap *shadow_map_;

  vector<Camera*> cameras_;
  int active_camera_;
  int light_camera_;
  Player *player_;

  double frame_time_;
};

#endif