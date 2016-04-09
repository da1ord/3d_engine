#ifndef FBO_H_
#define FBO_H_

class FBO {
 public:
  FBO();
  FBO(bool rbo, int colors, bool depth, int width, int height);
  ~FBO();
  void InitFBO();
  void CheckFBO();
  void InitRBO(int width, int height);
  void GenerateTextures(int colors, bool depth, int width, int height);
  void BindRead();
  void BindDraw();
  void Unbind();
  void SetReadBuffer(int i);

  GLuint fbo_;
  GLuint rbo_;
  GLuint color_attachment_[4];
  GLenum draw_buffers_[4];
  GLuint depth_attachment_;
  GLuint shader_id_;
};

#endif