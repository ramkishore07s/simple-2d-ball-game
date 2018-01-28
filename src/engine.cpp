#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include "ball.h"

bool detect_collision(Boundary a, Boundary b) {
  if( a.disk && b.disk ) {
    float x2 = (a.x - b.x) * (a.x - b.x);
    float y2 = (a.y - b.y) * (a.y - b.y);
    if( sqrt(x2 + y2) < a.r + b.r)
      return true;
    return false;
  }
  //..
  return false;
}

int where(glm::vec3 pos, float r, float speed, float speed_y) {
  float g = -2.0f;
  float t = -1.0f;
  const int ground = 0, air = 1, water = 2, trampoline = 3, left_edge = 4, right_edge = 5;
  if( pos.x <= 4.5 + r/2 && pos.x >= 2.5 - r/2 &&
      pos.y < t + r - speed_y && pos.y > t + r ) {
    if( speed_y < 0 )
      return trampoline;
    else
      return air;
  }
  else if( pos.x < 1.3 && pos.x > -1.3 ) {
    if( pos.y + speed_y < g + r )
      return water;
    else
      return air;
  }
  else if( pos.y < g + r + 0.04 && speed_y < 0)
    return ground;
  else
    return air;
}
  
      
