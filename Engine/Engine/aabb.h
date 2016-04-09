#ifndef AABB_H_
#define AABB_H_

class AABB {
 public:
  AABB();
  AABB(vec3 b1, vec3 b2);
  ~AABB();

  bool UpdateBounds(vec3 vert);
  bool RayIntersect(vec3 origin, vec3 direction, double &t);
  void MergeAABBs(AABB &box);
  vec3 GetCenter();
  vec3 GetSize();
  void Recalculate();
  void CalcCenter();
  void CalcSize();
  bool FitsInAABB(AABB &box);
  //NodeVisibility AABBInFrustum(glm::mat4 *mvp);
  void SetBounds(vec3 b1, vec3 b2);
  void MoveTo(vec3 pos);
  void MoveBy(vec3 diff);

  vec3 bounds_[2];
  vec3 center_;
  vec3 size_;
};

#endif