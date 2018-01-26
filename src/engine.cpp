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
