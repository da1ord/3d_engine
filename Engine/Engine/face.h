#ifndef FACE_H_
#define FACE_H_

struct Face
{
  Vertex *verts_[3];
  int indices_[3];
  glm::vec3 center_;
};

#endif