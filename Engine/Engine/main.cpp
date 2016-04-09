// Application entry point

#include "main.h"

int main( int argc, char * argv[] )
{
  SceneSettings *scene_settings = new SceneSettings();
  
  Scene *scene = new Scene();

  scene->Run();
  
  delete scene;
  scene = NULL;

  return 0;
}