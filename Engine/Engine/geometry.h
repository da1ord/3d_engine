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
  GLuint num_verts_;
  GLuint num_indices_;
  GLuint num_faces_;
  GLuint vao_;
  GLuint vbo_;
  GLuint ibo_;
};

#endif