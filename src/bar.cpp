#include "bar.h"
#include "main.h"

Bar::Bar(float radius) {
  this->scale = radius;
  static const float vbd[] = {
    1, 1.5, 0,
    1, 1, 0,
    -1, 1.5, 0,

    -1, 1.5, 0,
    -1, 1, 0,
    1, 1, 0,
  };
  this->object = create3DObject(GL_TRIANGLES, 6, vbd, COLOR_BROWN, GL_FILL);
  float angles[] = {0, 45, 135, 180, 225, 315};
  this->angle = rand() % 360;
  //  this->angle = 135;
}

void Bar::draw(glm::mat4 VP, glm::vec3 pos) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) ((this->angle) * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(pos));
  rotate = rotate * glm::translate(glm::vec3(0, 0, 1));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = this->scale;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->object);
}

Boundary Bar::boundary(glm::vec3 pos) {
  Boundary b;
  b.disk = false;
  b.x = pos.x;
  b.y = pos.y;
  b.angle = this->angle;
  b.r = this->scale;
  return b;
}
