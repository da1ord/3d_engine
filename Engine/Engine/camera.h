#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
 public:
  Camera();
  ~Camera();
  void Move(vec3 position);
  void Move(float pos_x, float pos_y, float pos_z);
  void Rotate(vec3 rotation);
  void Rotate(float rot_x, float rot_y, float rot_z);
  void UpdateView();
  mat4 *GetView();
  
  glm::vec3 position_;
  glm::vec3 look_at_;
  glm::vec3 rotation_;
  glm::mat4 view_matrix_;
  glm::mat4 proj_matrix_;
  bool is_player_;
};

#endif