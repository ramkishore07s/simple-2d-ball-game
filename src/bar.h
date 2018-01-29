#include "main.h"

#ifndef BAR_H
#define BAR_H


class Bar {
 public:
  Bar() {}
  Bar(float radius);
  float scale, angle;
  
  void draw(glm::mat4 VP, glm::vec3 pos);
  Boundary boundary(glm::vec3 pos);

 private:
  VAO* object;
};

#endif
