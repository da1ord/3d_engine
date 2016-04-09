#ifndef PLAYER_H_
#define PLAYER_H_

class Player {
 public:
  int camera_id_;
  Object model_;
  glm::vec3 position_;
  glm::vec2 rotation_;
  PxCapsuleController * nxCCT_;
  int active_gun_;
  vector<Weapon> arms_;
};

#endif