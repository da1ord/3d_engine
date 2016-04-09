#include "main.h"

Camera::Camera() {
  position_ = vec3(0, 0, 0);
  rotation_ = vec3(0, 0, 0);
  look_at_ = vec3(0.0f, 0.0f, 1.0f);
  is_player_ = false;
}

Camera::~Camera() {
}

void Camera::Move(vec3 position) {
  position_ = position;
}

void Camera::Move(float pos_x, float pos_y, float pos_z) {
  position_ = vec3(pos_x, pos_y, pos_z);
}

void Camera::Rotate(vec3 rotation) {
  rotation_ = rotation;
  
  look_at_.x = sin(rad(rotation_.y)) * cos(rad(rotation_.x));
  look_at_.y = -sin(rad(rotation_.x));
  look_at_.z = cos(rad(rotation_.y)) * cos(rad(rotation_.x));
}

void Camera::Rotate(float rot_x, float rot_y, float rot_z) {
  rotation_ = vec3(rot_x, rot_y, rot_z);
  
  look_at_.x = sin(rad(rotation_.y)) * cos(rad(rotation_.x));
  look_at_.y = -sin(rad(rotation_.x));
  look_at_.z = cos(rad(rotation_.y)) * cos(rad(rotation_.x));
}

void Camera::UpdateView() {
  view_matrix_ = mat4(1);
  view_matrix_ = glm::rotate(view_matrix_, rad(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
  view_matrix_ = glm::rotate(view_matrix_, rad(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
  view_matrix_ = glm::translate(view_matrix_, -position_);
}

mat4 *Camera::GetView() {
  UpdateView();
  
  return &view_matrix_;
}
