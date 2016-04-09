#ifndef SCENE_SETTINGS_H_
#define SCENE_SETIINGS_H_

class SceneSettings
{
public:
  SceneSettings();
  ~SceneSettings();
  void LoadSettings();
  void SaveSettings();

  Video video_;
  Audio audio_;
  Mouse mouse_;
  Controls controls_;

private:
};

#endif