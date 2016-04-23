#include "main.h"

ShadowMap::ShadowMap() {
  ShadowMap(3, 0.85f, 0.1f, 1000.0f, 1920, 1920);
}

ShadowMap::ShadowMap(unsigned int csm_levels, float weight, float near_plane, 
    float far_plane, int width, int height) {
  csm_levels_ = csm_levels;
  shadow_vp_mats_ = vector<mat4>(csm_levels_);
  frustum_points_ws_ = vector<vec4>(csm_levels_ * 8);
  frustum_points_ls_ = vector<vec4>(csm_levels_ * 8);
  far_planes_ = vector<float>(csm_levels_ + 1);
  width_ = width;
  height_ = height;
  
  weight_ = weight;
  bias_mat_ = mat4(0.5,   0,     0, 0,
                     0,   0.5,   0, 0,
                     0,     0, 0.5, 0,
                     0.5, 0.5, 0.5, 1);

  CalcPlanes(near_plane, far_plane);
  GenerateShadowTexture();
  GenerateFrameBuffer();
  shadow_shader_ = Utility::LoadShaders("shaders/shadow.vert", 
    "shaders/shadow.frag", "");
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::CalcPlanes(float near_plane, float far_plane) {
  far_planes_[0] = near_plane;
  for (unsigned int i = 0; i < csm_levels_; i++) {
    float cLog = near_plane * powf(far_plane / near_plane, 
      (float)(i + 1) / csm_levels_);
    float cUni = near_plane + (far_plane - near_plane) * 
      (float)(i + 1) / csm_levels_;
      
    far_planes_[i + 1] = weight_ * cLog + (1.0f - weight_) * cUni;
    
    float nearPlane = 0.1f;
    if (i > 0) {
      nearPlane = far_planes_[i];
    }
  }
}

void ShadowMap::GenerateShadowTexture() {
  glGenTextures(1, &shadow_texture_);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadow_texture_);
  
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, 
    width_, height_, csm_levels_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ShadowMap::GenerateFrameBuffer() {
  glGenFramebuffers(1, &depth_buffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, depth_buffer_);
  
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
	
  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
    shadow_texture_, 0, 0);

  int ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (ret == GL_FRAMEBUFFER_COMPLETE) {
    cout << "Shadowmap FBO created." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
    cout << "Shadowmap FBO missing attachment." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
    cout << "Shadowmap FBO incomplete attachment." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_UNSUPPORTED) {
    cout << "Shadowmap FBO unsupported." << endl;
  }
  else {
    cout << "Shadowmap FBO failed." << endl;
  }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}