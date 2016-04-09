#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

class ShadowMap {
 public:
  ShadowMap();
  ShadowMap(unsigned int csm_levels, float weight, float near, float far, 
    int width, int height);
  ~ShadowMap();

  void CalcPlanes(float near_plane, float far_plane);
  void GenerateShadowTexture();
  void GenerateFrameBuffer();

 //private:
  unsigned int csm_levels_;
  vector<mat4> shadow_vp_mats_;
  vector<vec4> frustum_points_ws_;
  vector<vec4> frustum_points_ls_;
  vector<float> far_planes_;
  float weight_;
  mat4 bias_mat_;
  int width_;
  int height_;
  unsigned int shadow_texture_;
  unsigned int depth_buffer_;
  unsigned int shadow_shader_;
};

#endif