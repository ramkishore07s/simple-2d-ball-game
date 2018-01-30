#include "magnet.h"
#include "main.h"

Magnet::Magnet(bool a) {
  this->on = false;
  this->right = false;
  this->time = 0;
  this->blackout = 0;
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

  static const GLfloat vbd_2[] = {
    0.8, 0, 0,
    0.6, 0, 0,
    0.8, 0.5, 0,

    0.8, 0.5, 0,
    0.6, 0.4, 0,
    0.6, 0,0,

    -0.8, 0, 0,
    -0.6, 0, 0,
    -0.8, 0.5, 0,

    -0.8, 0.5, 0,
    -0.6, 0.4, 0,
    -0.6, 0, 0,
  };

  //  static const vb
  this->object_red = create3DObject(GL_TRIANGLES, 36, vbd_1, COLOR_MAGNET_RED, GL_FILL);
  this->object_null = create3DObject(GL_TRIANGLES, 36, vbd_1, COLOR_BACKGROUND, GL_FILL);
  this->object_grey = create3DObject(GL_TRIANGLES, 12, vbd_2, COLOR_BLACK, GL_FILL);
}

void Magnet::draw_1(glm::mat4 VP, glm::vec3 eye) {
  Matrices.model = glm::mat4(1.0f);
  float theta = -90;
  if( !right )
    theta = 90;
    
  glm::mat4 rotate    = glm::rotate((float) (theta * M_PI / 180.0f),
				    glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(eye.x, 2, -1));    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 1));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 0.8;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->object_red);
}

void Magnet::draw_2(glm::mat4 VP, glm::vec3 eye) {
  float theta = -90;
  if( !right )
    theta = 90;

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (theta * M_PI / 180.0f),
				    glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(eye.x, 2, -1));    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 1));

  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 0.6;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->object_null);
}

void Magnet::draw_3(glm::mat4 VP, glm::vec3 eye) {
  float theta = 90;
  if( !right )
    theta = -90;

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (theta * M_PI / 180.0f),
				    glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(eye.x, 2, -1));    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 1));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 1.0;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->object_grey);
}

void Magnet::draw(glm::mat4 VP, glm::vec3 eye) {
  if( !right)
    eye.x -= 3.3;
  else
    eye.x += 3.3;
  draw_1(VP, eye);
  draw_2(VP, eye);
  draw_3(VP, eye);
}

bool Magnet::is_magnet_on(int score, int level3) {
  if( score >= level3 ) {
    if( this->time == 0 ) {
      if( this->blackout == 0 ) {
	if( rand() %10 == 0 ) {
	  this->right = !this->right;
	  this->time = 500;
	  this->blackout = 600;
	  return true;
	}
      }
      else 
	--this->blackout;
    }
    if( this->time > 0 ) {
      --this->time;
      return true;
    }
  }
  return false;
}
    
