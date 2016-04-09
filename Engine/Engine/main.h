//==============================
// List of application includes
//==============================
#ifndef MAIN_H_
#define MAIN_H_

//==========Defines=========
#define NX_GRAVITY -9.81f
#define rad( a ) ( ( a ) * 3.141592f / 180.0f )

//==========System==========
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

//==========OpenGL==========
#include <gli\gli.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <ogl\glew.h>
//#define GLFW_INCLUDE_GLU
#include <ogl\glfw3.h>

using namespace glm;
using namespace gli;

#define _GLFW_WIN32
#define _GLFW_WGL
#define _GLFW_USE_OPENGL

//==========PhysX===========
#include <physx\PxPhysicsAPI.h>

using namespace physx;

//==========Custom==========
// Settings
#include "video.h"
#include "audio.h"
#include "mouse.h"
#include "controls.h"
#include "scene_settings.h"
// Scene graph
#include "aabb.h"
#include "node.h"
#include "scene_graph.h"
#include "quadtree.h"
#include "octree.h"
// Object
#include "shader_uniforms.h"
#include "material.h"
#include "vertex.h"
#include "face.h"
#include "geometry.h"
#include "model.h"
#include "object.h"
// Scene
#include "weapon.h"
#include "gun.h"
#include "player.h"
#include "camera.h"
#include "shadow_map.h"
#include "fbo.h"

#include "scene.h"

#include "utility.h"

#endif