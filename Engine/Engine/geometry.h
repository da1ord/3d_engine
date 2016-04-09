#ifndef GEOMETRY_H_
#define GEOMETRY_H_

struct Geometry
{
  Geometry();
  Geometry(string &file, AABB *aabb);
  ~Geometry();
  void LoadGeometryOBJ(string &file, AABB *aabb);
  void LoadGeometry(string &file, AABB *aabb);
  void CreateBufferObjects();

  Vertex *vertices_;
  GLuint *indices_;
  vector<Face*> faces_;
  int num_verts_;
  int num_indices_;
  int num_faces_;
  GLuint vao_;
  GLuint vbo_;
  GLuint ibo_;
};

#endif