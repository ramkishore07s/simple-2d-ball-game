#include "main.h"

#ifndef PP_H
#define PP_H

class Porcupine {
 public:
  Porcupine() {};
  Porcupine(int i);

  glm::vec3 position;
  float speed;
  bool move;
  void draw(glm::mat4 VP);
  Boundary boundary();
  void tick();
 private:
  VAO *object;
};

#endif
