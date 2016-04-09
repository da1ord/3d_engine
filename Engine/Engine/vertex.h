#ifndef VERTEX_H_
#define VERTEX_H_

struct Vertex
{
  glm::vec3 position_;
  glm::vec3 normal_;
  glm::vec2 tex_uv_;
  glm::vec3 tangent_;
  glm::vec3 binormal_;
  char padding_[2];
};

#endif