#include "main.h"

SceneSettings::SceneSettings() {
  video_ = Video();
  video_.near_plane_ = 0.1f;
  video_.far_plane_ = 1000.0f;
  video_.fov_ = 60.f;
  video_.width_ = 1024;
  video_.height_ = 600;
  video_.aspect_ratio_ = (float)video_.width_ / (float)video_.height_;
  video_.fsaa_ = 1;
  video_.fullscreen_ = false;
}

SceneSettings::~SceneSettings() {
}

void SceneSettings::LoadSettings() {
}

void SceneSettings::SaveSettings() {
}