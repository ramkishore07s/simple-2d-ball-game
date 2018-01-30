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

glm::vec3 dot_prod(float x, float y, float x1, float y1) {
  return glm::vec3(x*x1, y*y1, 0);
}

bool detect_collision(Boundary a, Boundary b) {
  if( b.disk ) {
    float x2 = (a.x - b.x) * (a.x - b.x);
    float y2 = (a.y - b.y) * (a.y - b.y);
    if( sqrt(x2 + y2) < a.r + b.r)
      return true;
    return false;
  }
  return false;
}

bool detect_collision_plank(Boundary a, Boundary b, double* speed, double* speed_y) {

  glm::mat4 translate = glm::translate(glm::vec3(-b.x, -b.y, 0));
  glm::mat4 rotate = glm::rotate((float)((-b.angle) * M_PI / 180.0f), glm::vec3(0,0,1));
  glm::mat4 y_adjust = glm::translate(glm::vec3(0, -b.r, 0));
  glm::mat4 trans = y_adjust * rotate * translate;

  glm::vec4 player = trans * glm::vec4(a.x, a.y, 0, 1);
  glm::vec4 velocity = trans * glm::vec4(*speed, *speed_y, 0, 0);
  
  double theta = (360 - b.angle) * M_PI / 180.0f;
  double vx = *speed, vy = *speed_y;
  double new_vx = vx*cos(theta) - vy * sin(theta),
    new_vy = vy * cos(theta) + vx * sin(theta);
  
  if( player.x <= b.r + a.r/2 + 0.01 && player.x >= -b.r-a.r/2 -0.01 )
    if( player.y <= a.r + b.r/2 + abs(vy) && player.y > -a.r/2 - abs(vy) ) {
      std::cout<<"collided "<<player.x<<" "<<player.y<<"\n";
      velocity.y = -velocity.y;
      velocity = glm::rotate((float)((b.angle)*M_PI / 180.0f), glm::vec3(0,0,1)) * velocity;
      *speed = velocity.x;
      *speed_y = velocity.y;
      return true;
    }

  if( player.y <= b.r/2 + a.r/2 && player.y >= -0.001 )
    if( player.x <= b.r + a.r + 0.001 && player.x >= -b.r -a.r -0.001) {
      velocity.x = -velocity.x;
      velocity = glm::rotate((float)((b.angle)*M_PI / 180.0f), glm::vec3(0,0,1)) * velocity;
      *speed = velocity.x;
      *speed_y = velocity.y;
      return true;
    }

  if( sqrt(pow(abs(player.x)-b.r, 2) + pow(player.y-b.r/2,2)) < 0.01) {
    velocity.x = -velocity.x;
    velocity.y = -velocity.y;
    velocity = glm::rotate((float)((b.angle)*M_PI / 180.0f), glm::vec3(0,0,1)) * velocity;
    *speed = velocity.x;
    *speed_y = velocity.y;
    return true;
  }
  if( sqrt(pow(abs(player.x)-b.r, 2) + pow(player.y,2)) < 0.01 + a.r) {
    velocity.x = -velocity.x;
    velocity.y = -velocity.y;
    velocity = glm::rotate((float)((b.angle)*M_PI / 180.0f), glm::vec3(0,0,1)) * velocity;
    *speed = velocity.x;
    *speed_y = velocity.y;
    return true;
  }

  return false;
}

bool in_water(glm::vec3 pos, float r, float speed, float speed_y) {
  if( pos.x <= 1.3 - r && pos.x >= -1.3 + r )
    if( pos.y <= -2 ) {
      return true;
    }
  return false;
}

bool in_water_ground(glm::vec3 pos, float r, float speed, float speed_y) {
  if( sqrt(pow(pos.y + 2,2) + pow(pos.x, 2)) <= 1.3 - r + 0.1
      && sqrt(pow(pos.y + 2,2) + pow(pos.x, 2)) >= 1.3 - r - 0.01 )
    if( pos.y <= -2 )  {
      return true;
    }
  return false;
}

bool on_trampoline(glm::vec3 pos, float r, float speed, float speed_y) {
  if( pos.x <= 4.5 + r/2 && pos.x >= 2.5 - r/2 &&
      pos.y < -1 + r - speed_y && pos.y > -1 + r )
    if( speed_y < 0 ) {
      return true;
    }
  return false;
}

bool in_ground(glm::vec3 pos, float r, float speed, float speed_y) {
  if( pos.x <= -1.3 || pos.x >= 1.3 )
    if( pos.y < -1.99 + r && speed_y < 0 ) {
      return true;
    }
  return false;
}

bool in_right_edge(glm::vec3 pos, float r, float speed, float speed_y) {
  if( pos.x < 1.3 && pos.x > 1.3 -r &&  pos.y < -1.99)
    if( speed_y <= 0.001 )
      return true;
  return false;
}

bool in_left_edge(glm::vec3 pos, float r, float speed, float speed_y) {
  if( pos.x > -1.3 && pos.x < -1.3 + r && pos.y < -1.99)
    if( speed_y <= 0.001)
      return true;
  return false;
}

int where(glm::vec3 pos, float r, float speed, float speed_y) {
  float g = -2.0f;
  float t = -1.0f;
  const int ground = 0, air = 1, water = 2, trampoline = 3, left_edge = 4, right_edge = 5, w_ground = 6;
  if( in_right_edge(pos, r, speed, speed_y) )
    return right_edge;
  if( in_left_edge(pos, r, speed, speed_y) )
    return left_edge;
  if( in_ground(pos, r, speed, speed_y) )
    return ground;
  if( in_water_ground(pos, r, speed, speed_y) )
    return w_ground;
  if( in_water(pos, r, speed, speed_y) )
    return water;
  if( on_trampoline(pos, r, speed, speed_y) )
    return trampoline;
  return air;
}
  
      
