#include "ball.h"
#include "main.h"

void compute_next_vertex(int i, int n, float vbd[], float* theta, float radius) {
  vbd[3] = radius * cosf(*theta);
  vbd[4] = radius * sinf(*theta);

  if( i!= n-1 ) {
    *theta =  2.0f * 3.1415926f * float(i+1) / float(n);//get the current angle
    vbd[6] = radius * cosf(*theta);
    vbd[7] = radius * sinf(*theta);
  } else {
    vbd[6] = radius;
    vbd[7] = 0;
  }
}

Ball::Ball(float x, float y, float r, color_t c, bool dual) {
  this->position = glm::vec3(x, y, 0);
  this->rotation = 0;
  this->radius = r;
  speed = 0.01;
  speed_y = 0;
  this->color = c;
  this->dual = dual;
  this->speed = 0.01;
  this->speed_y = 0;

  static const GLfloat vbd_1[] = {
    0.0, 0.0, 0.0,
    1,0,0.0,
    0.965926,0.258819,0.0,
    0.0, 0.0, 0.0,
    0.965926,0.258819,0.0,
    0.866025,0.5,0.0,
    0.0, 0.0, 0.0,
    0.866025,0.5,0.0,
    0.707107,0.707107,0.0,
    0.0, 0.0, 0.0,
    0.707107,0.707107,0.0,
    0.5,0.866025,0.0,
    0.0, 0.0, 0.0,
    0.5,0.866025,0.0,
    0.258819,0.965926,0.0,
    0.0, 0.0, 0.0,
    0.258819,0.965926,0.0,
    -4.37114e-08,1,0.0,
  };
  static const GLfloat vbd_2[] = {
    0.0, 0.0, 0.0,
    -4.37114e-08,1,0.0,
    -0.258819,0.965926,0.0,
    0.0, 0.0, 0.0,
    -0.258819,0.965926,0.0,
    -0.5,0.866026,0.0,
    0.0, 0.0, 0.0,
    -0.5,0.866026,0.0,
    -0.707107,0.707107,0.0,
    0.0, 0.0, 0.0,
    -0.707107,0.707107,0.0,
    -0.866025,0.5,0.0,
    0.0, 0.0, 0.0,
    -0.866025,0.5,0.0,
    -0.965926,0.258819,0.0,
    0.0, 0.0, 0.0,
    -0.965926,0.258819,0.0,
    -1,-8.74228e-08,0.0,
  };
  static const GLfloat vbd_3[] = {
    0.0, 0.0, 0.0,
    -1,-8.74228e-08,0.0,
    -0.965926,-0.258819,0.0,
    0.0, 0.0, 0.0,
    -0.965926,-0.258819,0.0,
    -0.866025,-0.5,0.0,
    0.0, 0.0, 0.0,
    -0.866025,-0.5,0.0,
    -0.707107,-0.707107,0.0,
    0.0, 0.0, 0.0,
    -0.707107,-0.707107,0.0,
    -0.5,-0.866025,0.0,
    0.0, 0.0, 0.0,
    -0.5,-0.866025,0.0,
    -0.258819,-0.965926,0.0,
    0.0, 0.0, 0.0,
    -0.258819,-0.965926,0.0,
    -4.64912e-07,-1,0.0,
  };
  static const GLfloat vbd_4[] = {
    0.0, 0.0, 0.0,
    -4.64912e-07,-1,0.0,
    0.258819,-0.965926,0.0,
    0.0, 0.0, 0.0,
    0.258819,-0.965926,0.0,
    0.499999,-0.866026,0.0,
    0.0, 0.0, 0.0,
    0.499999,-0.866026,0.0,
    0.707107,-0.707107,0.0,
    0.0, 0.0, 0.0,
    0.707107,-0.707107,0.0,
    0.866025,-0.5,0.0,
    0.0, 0.0, 0.0,
    0.866025,-0.5,0.0,
    0.965926,-0.258819,0.0,
    0.0, 0.0, 0.0,
    0.965926,-0.258819,0.0,
    1,0.0,0.0,
  };
  if( dual ) {
    this->o1 = create3DObject(GL_TRIANGLES, 18, vbd_1, COLOR_RED, GL_FILL);
    this->o2 = create3DObject(GL_TRIANGLES, 18, vbd_2, COLOR_YELLOW, GL_FILL);
    this->o3 = create3DObject(GL_TRIANGLES, 18, vbd_3, COLOR_BLUE, GL_FILL);
    this->o4 = create3DObject(GL_TRIANGLES, 18, vbd_4, COLOR_BROWN, GL_FILL);
  }
  else {
    this->o1 = create3DObject(GL_TRIANGLES, 18, vbd_1, COLOR_RED, GL_FILL);
    this->o2 = create3DObject(GL_TRIANGLES, 18, vbd_2, COLOR_RED, GL_FILL);
    this->o3 = create3DObject(GL_TRIANGLES, 18, vbd_3, COLOR_RED, GL_FILL);
    this->o4 = create3DObject(GL_TRIANGLES, 18, vbd_4, COLOR_RED, GL_FILL);
  }

}

void Ball::set_position(float x, float y) {
  this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
  this->position.x += this->speed;
  this->rotate_ball(this->speed);
}

Boundary Ball::boundary() {
  Boundary b;
  b.x = this->position.x;
  b.y = this->position.y;
  b.r = this->radius;
  b.disk = true;
  b.h = 0;
  return b;
}

void Ball::rotate_ball(float disp) {
  this->rotation -= disp * 180 / (2*3.14*this->radius);
}

void Ball::draw(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (this->position);    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = this->radius;
  s[3][3] = 1.0f;
  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->o1);
  draw3DObject(this->o2);
  draw3DObject(this->o3);
  draw3DObject(this->o4);
}
