#include "main.h"

Scene::Scene() {
  scene_settings_ = new SceneSettings();

  Init();
}

Scene::Scene(SceneSettings *scene_settings) {
  scene_settings_ = scene_settings;

  Init();
}

Scene::~Scene() {
}
  
void Scene::Init() {
  InitOGL();
  InitPX();
  
  string level = "level.txt";
  LoadLevel(level);

  player_ = new Player();
  Camera *c = new Camera();
  Camera *l = new Camera();
  c->is_player_ = true;
  
  player_->camera_id_ = 0;
  cameras_.push_back(c);
  cameras_.push_back(l);

  active_camera_ = 0;
  light_camera_ = 1;

  cameras_[active_camera_]->proj_matrix_ = 
    perspective(rad(scene_settings_->video_.fov_), 
    scene_settings_->video_.aspect_ratio_, scene_settings_->video_.near_plane_,
    scene_settings_->video_.far_plane_);

  cameras_[active_camera_]->position_ = vec3(50.0f, 50.0f, 50.0f);
  cameras_[active_camera_]->view_matrix_ = lookAt(vec3(50.0f, 50.0f, 50.0f), 
   vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

  cameras_[light_camera_]->position_ = vec3(-500.0f, 500.0f, 500.0f);
  cameras_[light_camera_]->view_matrix_ = lookAt(vec3(-500.0f, 500.0, 500.0f), 
   vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

  shadow_map_ = new ShadowMap(3, 0.5f, scene_settings_->video_.near_plane_, 
    scene_settings_->video_.far_plane_, 2 * scene_settings_->video_.width_, 
    2 * scene_settings_->video_.height_);
}

void Scene::InitOGL() {
  cout << "Initializing OpenGL ..." << endl;
  
  // GLFW initialization
  if (!glfwInit()) {
    cerr << "Failed to initialize GLFW." << endl;
    exit(EXIT_FAILURE);
  }
  else {
    cout << "GLFW initialized." << endl;
  }
  
  // Context version setup
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // FSAA setup
  if (scene_settings_->video_.fsaa_ > 1) {
    glfwWindowHint(GLFW_SAMPLES, scene_settings_->video_.fsaa_);
  }

  // Window creation
  if (scene_settings_->video_.fullscreen_) {
    window_ = glfwCreateWindow(scene_settings_->video_.width_, 
      scene_settings_->video_.height_, "Engine", glfwGetPrimaryMonitor(), 
      NULL);
  }
  else {
    window_ = glfwCreateWindow(scene_settings_->video_.width_, 
      scene_settings_->video_.height_, "Engine", NULL, NULL);
  }

  glfwMakeContextCurrent(window_);

  if (!window_) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  // GLEW initialization
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW." << endl;
    exit(EXIT_FAILURE);
  }
  else {
    cout << "GLEW initialized." << endl;
  }
  
  //glfwSetKeyCallback(window_, KeyboardCallback);
}

void Scene::InitPX() {
}

void Scene::KeyboardCallback() {
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window_, GL_TRUE);
  }

  // Player
  if (glfwGetKey(window_, 'W') == GLFW_PRESS) {
    vec3 pos = cameras_[active_camera_]->position_;
    //cameras_[active_camera_]->Move(pos.x + frame_time_, pos.y, pos.z);
    cameras_[active_camera_]->position_.x += 
      cameras_[active_camera_]->look_at_.x;// * moveSpeed;
    cameras_[active_camera_]->position_.y += 
      cameras_[active_camera_]->look_at_.y;// * moveSpeed;
    cameras_[active_camera_]->position_.z -= 
      cameras_[active_camera_]->look_at_.z;// * moveSpeed;
  }
  if (glfwGetKey(window_, 'S') == GLFW_PRESS) {
    vec3 pos = cameras_[active_camera_]->position_;
    //cameras_[active_camera_]->Move(pos.x - frame_time_, pos.y, pos.z);
    cameras_[active_camera_]->position_.x -= 
      cameras_[active_camera_]->look_at_.x;// * moveSpeed;
    cameras_[active_camera_]->position_.y -= 
      cameras_[active_camera_]->look_at_.y;// * moveSpeed;
    cameras_[active_camera_]->position_.z += 
      cameras_[active_camera_]->look_at_.z;// * moveSpeed;
  }
  if (glfwGetKey(window_, 'A') == GLFW_PRESS) {
    vec3 pos = cameras_[active_camera_]->position_;
    //cameras_[active_camera_]->Move(pos.x, pos.y, pos.z - frame_time_);
    cameras_[active_camera_]->position_.x -= 
      cos(rad(cameras_[active_camera_]->rotation_.y));// * moveSpeed;
    cameras_[active_camera_]->position_.z -= 
      sin(rad(cameras_[active_camera_]->rotation_.y));// * moveSpeed;
  }
  if (glfwGetKey(window_, 'D') == GLFW_PRESS) {
    vec3 pos = cameras_[active_camera_]->position_;
    //cameras_[active_camera_]->Move(pos.x, pos.y, pos.z + frame_time_);
    cameras_[active_camera_]->position_.x += 
      cos(rad(cameras_[active_camera_]->rotation_.y));// * moveSpeed;
    cameras_[active_camera_]->position_.z += 
      sin(rad(cameras_[active_camera_]->rotation_.y));// * moveSpeed;
  }
  if (glfwGetKey(window_, 'C') == GLFW_PRESS) {
    active_camera_++;
    
    if (active_camera_ == light_camera_) {
      active_camera_++;
    }
    if (active_camera_ >= cameras_.size()) {
      active_camera_ = 0;
    }
  }

  cameras_[active_camera_]->UpdateView();//active_camera_
}

void Scene::MouseMoveCallback() {
  double mx, my;
  glfwGetCursorPos(window_, &mx, &my);
  
  int h_width = scene_settings_->video_.width_ / 2;
  int h_height = scene_settings_->video_.height_ / 2;
  double diffx = (double)(mx - h_width);// * settings_.mouse_sensitivity_;
  double diffy = (double)(my - h_height);// * settings_.mouse_sensitivity_;
  
  vec3 rot = cameras_[player_->camera_id_]->rotation_;//active_camera_

  if ((rot.x + diffy) >= -90 && (rot.x + diffy) <= 90) {
    rot.x += diffy;
    //rot.x = ( GLfloat )( rot.x % 360.0f );
  }
  rot.y += diffx;
  //rot.y = ( GLfloat )( rot.y % 360.0f );
   
  cameras_[player_->camera_id_]->Rotate(rot.x, rot.y, rot.z);//active_camera_
  glfwSetCursorPos(window_, h_width, h_height);
}

void Scene::MouseClickCallback() {
  if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
  }
}

void Scene::MouseScrollCallback() {
}

void Scene::LoadLevel(string &level_file) {    
  //LoadMaterials(level_file);

  string line;
  ifstream level(level_file);
  if (level.is_open()) {
    while (getline(level, line)) {
      if (line.length() > 0) {
        if (line[0] == '#') {
          continue;
        }
        if (line[0] == 'p') { // ply
          int dot_pos = line.find_last_of(".");
          string file_name = line.substr(2, dot_pos - 1);

          string model_file = file_name + "ply";
          
          int slash_pos = file_name.find('/') + 1;
          string name = file_name.substr(slash_pos, file_name.length() - 
            slash_pos);

          Model *model = new Model(model_file);
          model->name_ = name;
          
          for (unsigned int i = 0; i < materials_.size(); i++) {
            if (model->name_ == materials_[i]->name_) {
              model->material_ = materials_[i];
            }
          }
          Object *o = new Object();
          o->name_ = model->name_;
          o->models_.push_back(model);
          static_objects_.push_back(o);
        }
        else if (line[0] == 'o') { // obj
          int dot_pos = line.find_last_of(".");
          string file_name = line.substr(2, dot_pos - 1);

          string model_file = file_name + "obj";
          
          vector<Object*> objs = Utility::LoadModels(model_file, materials_);
          //TODO: temporary disables shadow for skydome
          if (objs[0]->name_ == "skydome")
            objs[0]->models_[0]->material_->has_norm_tex = 1;
          static_objects_.insert(static_objects_.end(), objs.begin(), 
            objs.end());
        }
      }
    }
    level.close();
  }
  for (unsigned int i = 0; i < static_objects_.size(); i++) {
    if (static_objects_[i]->name_ == "fsQuad") {
      fs_quad_ = static_objects_[i];
    }
    if (static_objects_[i]->name_ == "fsQuad2") {
      fs_quad2_ = static_objects_[i];
    }
    if (static_objects_[i]->name_ == "island") {
      terrain_ = static_objects_[i];
    }
  }
}

void Scene::LoadMaterials(string &level_file) {
  string line;
  ifstream level(level_file);
  if (level.is_open()) {
    while (getline(level, line)) {
      if (line.length() > 0) {
        if (line[0] == '#') {
          continue;
        }
        if (line[0] == 'o') {
          int dot_pos = line.find_last_of(".");

          string file_name = line.substr(2, dot_pos - 1);

          string material_file = file_name + "mtl";
          vector<Material*> mats = Utility::LoadMaterial(material_file);
          materials_.insert(materials_.end(), mats.begin(), mats.end());
        }
      }
    }
    level.close();
  }
}

void Scene::Run() {
  double current_frame = glfwGetTime();
  double last_frame = glfwGetTime();

  glViewport(0, 0, scene_settings_->video_.width_, 
    scene_settings_->video_.height_);
  glClearColor(1.f, 1.f, 1.f, 1.f);
  

  mat4 vp = cameras_[active_camera_]->proj_matrix_ * 
    cameras_[active_camera_]->view_matrix_;
  mat4 mvp;

  //TODO: if depth enabled - problem - not rendering
  //FBO tmp_fbo = FBO(false, 1, false, scene_settings_->video_.width_,
  //  scene_settings_->video_.height_);

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_TEXTURE_2D_ARRAY);
  
  while (!glfwWindowShouldClose(window_)) {
    frame_time_ = glfwGetTime() - last_frame;
    last_frame = glfwGetTime();
    
    float fps = 1.f / frame_time_;
    stringstream title;
    title << "Engine - FPS: " << (int)fps;
    glfwSetWindowTitle(window_, title.str().c_str());

    KeyboardCallback();
    MouseMoveCallback();
    vp = cameras_[active_camera_]->proj_matrix_ * 
    cameras_[active_camera_]->view_matrix_;
    ///<===========SHADOWMAP GENERATION===========
    for (unsigned int i = 0; i < shadow_map_->csm_levels_; i++) {
      CalcShadowMatrices(i, shadow_map_->far_planes_[i], 
        shadow_map_->far_planes_[i + 1]);
      
      GenerateShadowMap(shadow_map_->shadow_vp_mats_[i], i);
      
      shadow_map_->shadow_vp_mats_[i] = shadow_map_->bias_mat_ * 
        shadow_map_->shadow_vp_mats_[i];
    }
    //===========SHADOWMAP GENERATION===========>


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (unsigned int m = 0; m < materials_.size(); m++) {
      glUseProgram(materials_[m]->shader_id_);
      for (unsigned int i = 0; i < static_objects_.size(); i++) {
        // TODO: remove quads from static_objects
        if (static_objects_[i]->name_ == "fsQuad" || static_objects_[i]->name_ == "fsQuad2") {
          continue;
        }
        for (unsigned int j = 0; j < static_objects_[i]->models_.size(); j++) {
          if (static_objects_[i]->models_[j]->IsSameShader(materials_[m]->shader_id_)) {
            mvp = vp * static_objects_[i]->models_[j]->model_matrix_;

            static_objects_[i]->models_[j]->BindUniforms(mvp, 
              cameras_[active_camera_]->view_matrix_, 
              shadow_map_->shadow_texture_, &shadow_map_->shadow_vp_mats_[0], 
              &shadow_map_->far_planes_[1]);
            static_objects_[i]->models_[j]->Draw(0);
          }
        }
      }
    }
    glUseProgram(0);
 
    glClear(GL_DEPTH_BUFFER_BIT);

    fs_quad_->models_[0]->DrawFSQuadTex(shadow_map_->shadow_texture_, 0, 0.1, vec2(0.5, 0));
    fs_quad_->models_[0]->DrawFSQuadTex(shadow_map_->shadow_texture_, 1, 0.1, vec2(0.7, 0));
    fs_quad_->models_[0]->DrawFSQuadTex(shadow_map_->shadow_texture_, 2, 0.1, vec2(0.9, 0));
    

    //glDisable(GL_TEXTURE_2D_ARRAY);
    //glDisable(GL_TEXTURE_2D);
    
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  glfwDestroyWindow(window_);
  glfwTerminate();
}

//void Scene::KeyboardCallback(GLFWwindow* window, int key, int scancode, 
//    int action, int mods) {
//  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//      glfwSetWindowShouldClose(window, GL_TRUE);
//}

void Scene::CalcShadowVPMats(float *farPlanes, mat4 *shadowVPMat, float weight) {
  for (unsigned int i = 0; i < shadow_map_->csm_levels_; i++) {
    CalcShadowMatrices(i, shadow_map_->far_planes_[i], shadow_map_->far_planes_[i + 1]);
  }
}

void Scene::CalcShadowMatrices(int csmLevel, float nearPlane, float farPlane) {
  // Get player view transformation
  mat4 lookMat = mat4(1.0f);
  lookMat = rotate(lookMat, rad(-cameras_[active_camera_]->rotation_.x), vec3(1.0f, 0.0f, 0.0f));
  lookMat = rotate(lookMat, rad(cameras_[active_camera_]->rotation_.y + 180), vec3(0.0f, 1.0f, 0.0f));
  lookMat = translate(lookMat, -cameras_[active_camera_]->position_);
  lookMat = inverse(lookMat);

  //lookMat = (cameras_[active_camera_]->view_matrix_);
  // Get player position
  vec4 pos = vec4(cameras_[active_camera_]->position_.x, 
    cameras_[active_camera_]->position_.y, 
    cameras_[active_camera_]->position_.z, 1.0);
  
  vec4 right = normalize(vec4(lookMat[0]));
  vec4 up = normalize(vec4(lookMat[1]));
  vec4 fw = normalize(vec4(lookMat[2]));

  vec4 cNear = pos + fw * nearPlane;
  vec4 cFar = pos + fw * farPlane;
  
  float hNear = 2.0f * tan(rad(scene_settings_->video_.fov_ / 2.0f)) * 
    nearPlane;
  float wNear = hNear * scene_settings_->video_.aspect_ratio_;
  float hFar = 2.0f * tan(rad(scene_settings_->video_.fov_ / 2.0f)) * 
    farPlane;
  float wFar = hFar * scene_settings_->video_.aspect_ratio_;
  
  // Get player view frustum points in WS
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 0] = 
    cFar + (up * (hFar / 2)) - (right * (wFar / 2)); // FTL
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 1] = 
    cFar + (up * (hFar / 2)) + (right * (wFar / 2)); // FTR
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 2] = 
    cFar - (up * (hFar / 2)) - (right * (wFar / 2)); // FBL
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 3] = 
    cFar - (up * (hFar / 2)) + (right * (wFar / 2)); // FBR

  shadow_map_->frustum_points_ws_[csmLevel * 8 + 4] = 
    cNear + (up * (hNear / 2)) - (right * (wNear / 2)); // NTL
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 5] = 
    cNear + (up * (hNear / 2)) + (right * (wNear / 2)); // NTR
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 6] = 
    cNear - (up * (hNear / 2)) - (right * (wNear / 2)); // NBL
  shadow_map_->frustum_points_ws_[csmLevel * 8 + 7] = 
    cNear - (up * (hNear / 2)) + (right * (wNear / 2)); // NBR
  
  // Crop view frustum points by scene AABB and get frustum center
  vec4 frustumCenter = vec4(0);
  for (unsigned int i = 0; i < 8; i++) {
    // Do not/crop nearest any frustum?
    // (missing player shadow in situations vs visible shadows out of frustum) 
    //if (csmLevel > 5) 
    //{
    //frustumPointsWS[csmLevel * 8 + i].x = 
    //  glm::min(frustumPointsWS[csmLevel * 8 + i].x, sceneAABB->bounds[1].x);
    //frustumPointsWS[csmLevel * 8 + i].x = 
    //  glm::max(frustumPointsWS[csmLevel * 8 + i].x, sceneAABB->bounds[0].x);
    //
    //frustumPointsWS[csmLevel * 8 + i].y = 
    //  glm::min(frustumPointsWS[csmLevel * 8 + i].y, sceneAABB->bounds[1].y);
    //frustumPointsWS[csmLevel * 8 + i].y = 
    //  glm::max(frustumPointsWS[csmLevel * 8 + i].y, sceneAABB->bounds[0].y);

    //frustumPointsWS[csmLevel * 8 + i].z = 
    //  glm::min(frustumPointsWS[csmLevel * 8 + i].z, sceneAABB->bounds[1].z);
    //frustumPointsWS[csmLevel * 8 + i].z = 
    //  glm::max(frustumPointsWS[csmLevel * 8 + i].z, sceneAABB->bounds[0].z);
    //}

    frustumCenter += shadow_map_->frustum_points_ws_[csmLevel * 8 + i];
  }
  frustumCenter /= 8;

  
  shadow_map_->frustum_points_ls_[csmLevel * 8 + 0] = 
    cameras_[light_camera_]->view_matrix_ * 
    shadow_map_->frustum_points_ws_[csmLevel * 8 + 0];
  vec4 min = shadow_map_->frustum_points_ls_[csmLevel * 8 + 0];
  vec4 max = shadow_map_->frustum_points_ls_[csmLevel * 8 + 0];
  for (unsigned int i = 1; i < 8; i++) {
    shadow_map_->frustum_points_ls_[csmLevel * 8 + i] = 
      cameras_[light_camera_]->view_matrix_ * 
      shadow_map_->frustum_points_ws_[csmLevel * 8 + i];
	  if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].x > max.x)
		  max.x = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].x;
	  else if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].x < min.x)
		  min.x = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].x;
	  if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].y > max.y)
		  max.y = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].y;
	  else if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].y < min.y)
		  min.y = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].y;
	  if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].z > max.z)
		  max.z = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].z;
	  else if (shadow_map_->frustum_points_ls_[csmLevel * 8 + i].z < min.z)
		  min.z = shadow_map_->frustum_points_ls_[csmLevel * 8 + i].z;
  }

  float offset = scene_settings_->video_.far_plane_;// 100.0f - ok, should be farPlane TODO: ?! by frustum level (0 - 10, 1 - 20, ...)
  cameras_[light_camera_]->proj_matrix_ = ortho(min.x, max.x, min.y, max.y, 
    -max.z - offset, -min.z);
  
  
  shadow_map_->shadow_vp_mats_[csmLevel] = 
    cameras_[light_camera_]->proj_matrix_ * 
    cameras_[light_camera_]->view_matrix_;
}

void Scene::GenerateShadowMap(mat4 &shadowVP, int level) {
  glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_->depth_buffer_);

  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
    shadow_map_->shadow_texture_, 0, level);
  
  glViewport(0, 0, shadow_map_->width_, shadow_map_->height_);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //glCullFace(GL_FRONT);
  glUseProgram(shadow_map_->shadow_shader_);
  
  //mat4 mvp = dMV * player_->GetTransformMatrix();
  //glUniformMatrix4fv(glGetUniformLocation(shadow_map_->shadow_shader_, "MVP"), 1, GL_FALSE, &mvp[0][0]);
  //player_->Draw();

  //mvp = dMV * player_->GetGunTransformMatrix();
  //glUniformMatrix4fv(glGetUniformLocation(shadow_map_->shadow_shader_, "MVP"), 1, GL_FALSE, &mvp[0][0]);
  //player_->DrawGun();


  for (unsigned int i = 0; i < static_objects_.size(); i++) {
    if (static_objects_[i]->name_ == "skydome") {
      continue;
    }
    for (unsigned int j = 0; j < static_objects_[i]->models_.size(); j++) {
      mat4 mvp = shadowVP * static_objects_[i]->models_[j]->model_matrix_;
      glUniformMatrix4fv(glGetUniformLocation(shadow_map_->shadow_shader_, 
        "MVP"), 1, GL_FALSE, &mvp[0][0]);
      //static_objects_[i]->models_[j]->BindUniforms(mvp);
      static_objects_[i]->models_[j]->Draw(0);
    }
  }
  glUseProgram(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, scene_settings_->video_.width_, scene_settings_->video_.height_);
  //glCullFace(GL_BACK);
}