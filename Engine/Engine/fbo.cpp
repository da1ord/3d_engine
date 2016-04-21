#include "main.h"

FBO::FBO() {
}

FBO::FBO(bool rbo, int colors, bool depth, int width, int height) {
  InitFBO();
  GenerateTextures(colors, depth, width, height);
  if (rbo) {
    InitRBO(width, height);
  }
  CheckFBO();

  //shader_id_ = Utility::LoadShaders("shaders/shader.vert", 
  //  "shaders/shader.frag", "");
}

FBO::~FBO() {
}

void FBO::InitFBO() {
  // Create the FBO
  glGenFramebuffers(1, &fbo_); 
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FBO::CheckFBO() {
  // Check FBO
  GLenum ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (ret == GL_FRAMEBUFFER_COMPLETE) {
    cout << "FBO created." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
    cout << "FBO missing attachment." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
    cout << "FBO incomplete attachment." << endl;
  }
  else if (ret == GL_FRAMEBUFFER_UNSUPPORTED) {
    cout << "FBO unsupported." << endl;
  }
  else {
    cout << "FBO failed." << endl;
  }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::InitRBO(int width, int height) {
  //TODO: Attachment type, drawBuffer
  glGenRenderbuffers(1, &rbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
  glDrawBuffer(GL_NONE);
}

void FBO::GenerateTextures(int colors, bool depth, int width, int height) {
  if (colors > 0) {
    glGenTextures(colors, color_attachment_);

    for (int i = 0 ; i < colors; i++) {
      glBindTexture(GL_TEXTURE_2D, color_attachment_[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
        GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
        GL_TEXTURE_2D, color_attachment_[i], 0);
      draw_buffers_[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(colors, draw_buffers_);
  }

  // Depth texture
  if (depth) {
    glGenTextures(1, &depth_attachment_);
    glBindTexture(GL_TEXTURE_2D, depth_attachment_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, 
      GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
      GL_TEXTURE_2D, depth_attachment_, 0);
  }
}

void FBO::BindRead() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
}

void FBO::BindDraw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
}

void FBO::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::SetReadBuffer(int i) {
  glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
}