#include "main.h"

#ifndef BG_H
#define BG_H


class Background {
 public:
  Background() {};
  Background(bool t);
  void draw_ground(glm::mat4 VP, glm::vec3 target);
  void draw_water_body(glm::mat4 VP);
  void draw_background(glm::mat4 VP, glm::vec3 target);
  void draw_trampoline(glm::mat4 VP);
 private:
  VAO *land_object;
  VAO *water_object;
  VAO *t_support;
  VAO *t;
};

#endif //BG_H
