#include "main.h"
#include "background.h"

Background::Background(bool t) {
  static const GLfloat water_data[] = {
    0.0, 0.0, 0.0,
    -1,0.0,0.0,
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
  this->water_object = create3DObject(GL_TRIANGLES, 36, water_data, COLOR_WATER_BLUE, GL_FILL);
  static const GLfloat land_data[] = {
    -10, -2, 0,
    10,  -2, 0,
    -10, -8, 0,

    -10, -8, 0,
    10, -2, 0,
    10, -8, 0,
  };
  this->land_object = create3DObject(GL_TRIANGLES, 6, land_data, COLOR_LAND, GL_FILL);
  static const GLfloat t_support_data[] = {
    1, 1, 0,
    0.9, 1, 0,
    1, 0, 0,

    0.9, 1, 0,
    0.9, 0, 0,
    1, 0, 0,
    
    -1, 1, 0,
    -0.9, 1, 0,
    -1, 0, 0,

    -0.9, 1, 0,
    -0.9, 0, 0,
    -1, 0, 0,
  };
  this->t_support =  create3DObject(GL_TRIANGLES, 12, t_support_data, COLOR_BROWN, GL_FILL);
  static const GLfloat t_data[] = {
    0.9, 1, 0,
    0.9, 0.9, 0,
    -0.9, 1, 0,

    -0.9, 1, 0,
    -0.9, 0.9, 0,
    0.9, 0.9, 0,
  };
  this->t = create3DObject(GL_TRIANGLES, 6, t_data, COLOR_BLACK, GL_FILL);
  //  static const GLfloat mountain_data[] = {
  
}

void Background::draw_ground(glm::mat4 VP, glm::vec3 target) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(target.x , 0, 0));
  rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
  Matrices.model *= (translate * rotate);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->land_object);
}

void Background::draw_water_body(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(0,-2,0));    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
  glm::mat4 s;
  for(int i=0; i<4; ++i)
    for(int j=0; j<4; ++j)
      s[i][j] = 0;
  for(int i=0; i<3; ++i)
    s[i][i] = 1.3f;
  s[3][3] = 1.0f;

  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(this->water_object);
}

void Background::draw_trampoline(glm::mat4 VP) {
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 rotate    = glm::rotate((float) (0 * M_PI / 180.0f), glm::vec3(0, 0, 1));
  glm::mat4 translate = glm::translate (glm::vec3(3.5,-2,0));    // glTranslatef
  rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
  glm::mat4 s;
  Matrices.model *= (translate * rotate * s);
  glm::mat4 MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  draw3DObject(this->t_support);
  draw3DObject(this->t);
}
  
void Background::draw_background(glm::mat4 VP, glm::vec3 target) {
  draw_ground(VP, target);
  draw_water_body(VP);
  draw_trampoline(VP);
}
