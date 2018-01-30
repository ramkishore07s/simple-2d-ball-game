#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(int i) {
  this->speed = 0.01;
  if( i == 0 )
    this->position.x = -2;
  else
    this->position.x = 4;
  this->position.y = -2;

  static const GLfloat vbd[] = {
    1, 0, 0,
    0.5, 0, 0,
    0.75, 0.5, 0,

    0.5, 0, 0,
    0, 0, 0,
    0.25, 0.5, 0,

    -1, 0, 0,
    -0.5, 0, 0,
    -0.75, 0.5, 0,

    -0.5, 0, 0,
    0, 0, 0,
    -0.25, 0.5, 0,
  };

  this->object = create3DObject(GL_TRIANGLES, 12, vbd, COLOR_BLACK, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) ((0) * M_PI / 180.0f),
				    glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(this->position));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 0.3;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->object);
}

Boundary Porcupine::boundary() {
  Boundary b;
  b.x = this->position.x;
  b.y = this->position.y - 0.3;
  b.angle = 0;
  b.r = 0.3;
  b.disk = false;
  return b;
}

void Porcupine::tick() {
  this->position.x += this->speed;
  if( this->position.x < 0 ) {
    if( this->position.x > -0.4-1.3 || this->position.x < -5 )
      this->speed = -this->speed;
  }
  else {
    if( this->position.x < 1.3 + 0.4 || this->position.x > 5 )
      this->speed = -this->speed;
  }
}


    
