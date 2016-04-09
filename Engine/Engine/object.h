#ifndef OBJECT_H_
#define OBJECT_H_

class Object {
 public:
  Object();
  ~Object();
  void LoadModels(string &file);

  string name_;
  vector<Model*> models_;
  int model_count_;
  bool is_visible_;
  bool is_dynamic_;
};

#endif