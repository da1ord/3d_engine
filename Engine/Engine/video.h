#ifndef VIDEO_H_
#define VIDEO_H_

struct Video {
  int width_;
  int height_;
  bool fullscreen_;
  bool vsync_;
  int refresh_rate_;
  int fsaa_;
  float draw_distance_;
  float near_plane_;
  float aspect_ratio_;
  float far_plane_;
  float fov_;
  bool ssao_;
};

#endif