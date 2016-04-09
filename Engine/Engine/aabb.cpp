#include "main.h"

AABB::AABB() {
  bounds_[0].x = 1000.0;
  bounds_[0].y = 1000.0;
  bounds_[0].z = 1000.0;
  bounds_[1].x = -1000.0;
  bounds_[1].y = -1000.0;
  bounds_[1].z = -1000.0;

  Recalculate();
}

AABB::AABB(vec3 b1, vec3 b2) {
  bounds_[0] = b1;
  bounds_[1] = b2;

  CalcCenter();
  CalcSize();
}

AABB::~AABB() {
}

bool AABB::UpdateBounds(vec3 vert) {
  bool updated = false;
  if (vert.x > bounds_[1].x) {
    bounds_[1].x = vert.x;
    updated = true;
  }
  if (vert.x < bounds_[0].x) {
    bounds_[0].x = vert.x;
    updated = true;
  }

  if (vert.y > bounds_[1].y) {
    bounds_[1].y = vert.y;
    updated = true;
  }
  if (vert.y < bounds_[0].y) {
    bounds_[0].y = vert.y;
    updated = true;
  }
    
  if (vert.z > bounds_[1].z) {
    bounds_[1].z = vert.z;
    updated = true;
  }
  if (vert.z < bounds_[0].z) {
    bounds_[0].z = vert.z;
    updated = true;
  }
  return updated;
}

bool AABB::RayIntersect(vec3 origin, vec3 direction, double &t) {
  vec3 dirfrac;
  dirfrac.x = 1.0f / direction.x;
  dirfrac.y = 1.0f / direction.y;
  dirfrac.z = 1.0f / direction.z;
  
  vec3 lb = bounds_[0];
  vec3 rt = bounds_[1];
  float t1 = (lb.x - origin.x) * dirfrac.x;
  float t2 = (rt.x - origin.x) * dirfrac.x;
  float t3 = (lb.y - origin.y) * dirfrac.y;
  float t4 = (rt.y - origin.y) * dirfrac.y;
  float t5 = (lb.z - origin.z) * dirfrac.z;
  float t6 = (rt.z - origin.z) * dirfrac.z;

  float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
  float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

  // intersection behing camera
  if (tmax < 0) {
    t = tmax;
    return false;
  }

  // missed
  if (tmin > tmax) {
    t = tmax;
    return false;
  }

  t = tmin;
  return true;
}

void AABB::MergeAABBs(AABB &box) {
  bounds_[0].x = std::min(bounds_[0].x, box.bounds_[0].x);
	bounds_[0].y = std::min(bounds_[0].y, box.bounds_[0].y);
	bounds_[0].z = std::min(bounds_[0].z, box.bounds_[0].z);

	bounds_[1].x = std::max(bounds_[1].x, box.bounds_[1].x);
	bounds_[1].y = std::max(bounds_[1].y, box.bounds_[1].y);
	bounds_[1].z = std::max(bounds_[1].z, box.bounds_[1].z);
  
  CalcCenter();
  CalcSize();
}

vec3 AABB::GetCenter() {
  return center_;
}

vec3 AABB::GetSize() {
  return size_;
}

void AABB::Recalculate() {
  CalcCenter();
  CalcSize();
}

void AABB::CalcCenter() {
  center_.x = (bounds_[0].x + bounds_[1].x) / 2.0;
  center_.y = (bounds_[0].y + bounds_[1].y) / 2.0;
  center_.z = (bounds_[0].z + bounds_[1].z) / 2.0;
}

void AABB::CalcSize() {
  size_.x = bounds_[1].x - bounds_[0].x;
  size_.y = bounds_[1].y - bounds_[0].y;
  size_.z = bounds_[1].z - bounds_[0].z;
}

bool AABB::FitsInAABB(AABB &box) {
  if (box.bounds_[0].x <= bounds_[0].x && 
    box.bounds_[0].y <= bounds_[0].y && 
    box.bounds_[0].z <= bounds_[0].z && 
    box.bounds_[1].x >= bounds_[1].x && 
    box.bounds_[1].y >= bounds_[1].y && 
    box.bounds_[1].z >= bounds_[1].z) {
    return true;
  }
  else {
    return false;
  }
}

//NodeVisibility AABB::AABBInFrustum(glm::mat4 *mvp) {
//  int nv[6] = { 0, 0, 0, 0, 0, 0 };
//  int pv = 0;
//  for (float i = -1; i <= 1; i += 2) {
//    for (float j = -1; j <= 1; j += 2) {
//      for (float k = -1; k <= 1; k += 2) {
//        glm::vec4 clip = *mvp * 
//          glm::vec4(center_.x + size_.x / 2.0 * i,
//            center_.y + size_.y / 2.0 * j,
//            center_.z + size_.z / 2.0 * k, 1.0);
//
//        if (clip.x > clip.w) {
//          nv[0]++;
//        }
//        if (clip.x < -clip.w) {
//          nv[1]++;
//        }
//        if (clip.y > clip.w) {
//          nv[2]++;
//        }
//        if (clip.y < -clip.w) {
//          nv[3]++;
//        }
//        if (clip.z > clip.w) {
//          nv[4]++;
//        }
//        if (clip.z < -clip.w) {
//          nv[5]++;
//        }
//      }
//    }
//  }
//  for (int i = 0; i < 6; i++)  {       
//    if (nv[i] == 8) {
//      return kNODE_NOT_VISIBLE;
//    }
//    else if (nv[i] > 0) { 
//      pv++;
//    }
//  }
//  if (pv > 0) {
//    return kNODE_PARTIALLY_VISIBLE;
//  }
//  else {
//    return kNODE_VISIBLE;
//  }
//}

void AABB::SetBounds(vec3 b1, vec3 b2) {
  bounds_[0] = b1;
  bounds_[1] = b2;

  Recalculate();
}

void AABB::MoveTo(vec3 pos) {
  bounds_[0] = pos;
  bounds_[1] = pos + size_;
  
  CalcCenter();
}

void AABB::MoveBy(vec3 diff) {
  bounds_[0] += diff;
  bounds_[1] += diff;
  
  CalcCenter();
}

//void AABB::DrawAABB() {
//  DrawAABB(*this, vec3(255, 255, 255));
//}

//void AABB::DrawAABB(vec3 color) {
//  DrawAABB(*this, color);
//}

//void AABB::DrawAABB(mat4 &view, mat4 &projection) {
//  mat4 MVP = projection * view;// * transformation_matrix_;
//  glLoadIdentity();
//  glLoadMatrixf(value_ptr(MVP));
//
//  DrawAABB(*this, vec3(255, 255, 255));
//}

//void AABB::DrawAABB(AABB &aabb, vec3 color) {
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glDisable(GL_CULL_FACE);
//  
//  glBegin(GL_QUADS);
//  glColor3f(color.r, color.g, color.b);
//    
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//  
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//
//  
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//  
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//
//  
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[0].y, aabb.bounds_[0].z);
//
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//  glVertex3f(aabb.bounds_[0].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[1].z);
//  glVertex3f(aabb.bounds_[1].x, aabb.bounds_[1].y, aabb.bounds_[0].z);
//
//  glEnd();
//
//  glEnable(GL_CULL_FACE);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
