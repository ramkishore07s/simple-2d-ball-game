#include "main.h"

#ifndef MG_H
#define MG_H

class Magnet {
 public:
  Magnet() {};
  Magnet(bool a);

  bool on, right;
  int time, blackout;
  bool is_magnet_on(int score, int level3);
  void draw(glm::mat4 VP, glm::vec3 eye);
  void draw_1(glm::mat4 VP, glm::vec3 eye);
  void draw_2(glm::mat4 VP, glm::vec3 eye);
  void draw_3(glm::mat4 VP, glm::vec3 eye);
 private:
  VAO *object_red;
  VAO *object_null;
  VAO *object_grey;
};

#endif
