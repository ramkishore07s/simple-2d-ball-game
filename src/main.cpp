#include "main.h"
#include "timer.h"
#include "ball.h"
#include "background.h"
#include "bar.h"
#include "magnet.h"
#include "porcupine.h"
#include "numbers.h"
#define PI 3.14159265

using namespace std;

int score, level1, level2, level3;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
int collided = 0;
bool magnet_on;
Numbers number;

Ball ball[100], player, test;
Background bg;
Bar bar[20], t_bar;
Magnet magnet;
Porcupine p[2];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, w_v = 0;
Timer t60(1.0 / 60);
int no_of_balls = 100, pan_right, pan_left, p_auto, pan_up, pan_down;

glm::vec3 eye, up, target;
GLfloat ground_level = -2.0f;
const int ground = 0, air = 1, water = 2, trampoline = 3,
  left_edge = 4, right_edge = 5, w_ground = 6;
const float gravity = 0.001;
int type;

int level = 1;
bool pan_auto = true;

float dist(glm::vec3 pos, float x, float y) {
  return sqrt((pos.x * pos.x - x*x) + (pos.y * pos.y - y*y));
}
	       
void tick_input(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void draw() {
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (programID);
  if( pan_auto ) {
    if( player.position.x >= 1)
      eye.x = target.x = player.position.x -1;
    else if( player.position.x  <= -1)
      eye.x = target.x = player.position.x +1;
    else
      eye.x = target.x = 0;
    screen_zoom = 1.5/(player.position.y + 4) + 0.5;
    
    eye.y = target.y = player.position.y/3 - 0.3;
    
    if( abs(player.speed) > 0.02 && screen_zoom > 0.5 )
      screen_zoom -= 0.0005;
    else if (screen_zoom < 1)
      screen_zoom += 0.0005;
  }
  else {
    if( pan_right ) {
      eye.x += 0.1;
      target.x += 0.1;
    }
    else if(pan_left) {
      eye.x -= 0.1;
      target.x -= 0.1;
    }
    if( pan_up ) {
      eye.y += 0.1;
      target.y += 0.1;
    }
    else if( pan_down ) {
      if( eye.y > -1 ) {
	eye.y -= 0.1;
	target.y -= 0.1;
      }
    }    
  }
  
  reset_screen();
  Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane

  
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;  // MVP = Projection * View * Model
  if( magnet_on )
    magnet.draw(VP, eye);

  bg.draw_background(VP, target);

  for(int i=0; i<no_of_balls; ++i) {
    if( ball[i].position.x > player.position.x + 50 )
      ball[i].position.x = player.position.x - 50;
    ball[i].draw(VP);
    if( i%5 == 0 && score >= level1)
      bar[i/5].draw(VP, ball[i].position);
  }
  player.draw(VP);
  if( score >= level2 ) {
    p[0].draw(VP);
    p[1].draw(VP);
  }
  number.draw(VP, glm::vec3(eye.x, 1, -4), 0.2, score);
  // test.draw(VP);
  // t_bar.draw(VP, test.position);
}

void tick_elements() {
  for(int i=0; i<no_of_balls; ++i)
    ball[i].tick();
  player.tick();
  for(int i=0; i<2; ++i)
    p[i].tick();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
  /* Objects should be created before any other gl function and shaders */
  // Create the models
  score = 0;
  level1 = 10;
  level2 = 20;
  level3 = 30;
  eye = glm::vec3(0,0,3);
  target = glm::vec3(0,0,0);
  up = glm::vec3(0,1,0);
  srand(time(NULL));
  number = Numbers(true);
  player = Ball(1.5, ground_level + 2 + 0.3, 0.3, COLOR_RED, true);
  for(int i=0; i<2; ++i )  
    p[i] = Porcupine(i);
  player.speed = 0;
  magnet_on = false;
  for(int i=0; i<no_of_balls; ++i) {
    float  y = (float)(rand()%99)/33.0f + 0.4;
    float x = 3 - i + rand()%3;
    float r = (rand()/(float)RAND_MAX)/4 + 0.2;
    ball[i] = Ball(x, y, r, COLOR_RED, false);
    if( i%5 == 0 )
      bar[i/5] = Bar(r);
  }
  magnet = Magnet(true);

  test = Ball(5, 0.7, 0.3, COLOR_RED, false);
  test.speed = 0;
  t_bar = Bar(0.3);
  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
  // Get a handle for our "MVP" uniform
  Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


  reshapeWindow (window, width, height);

  // Background color of the scene
  glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 0.0, COLOR_BACKGROUND.b / 0.0, 0.0f); // R, G, B, A
  glClearDepth (1.0f);

  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);

  cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
  cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
  cout << "VERSION: " << glGetString(GL_VERSION) << endl;
  cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
  srand(time(0));
  int width  = 800;
  int height = 800;

  window = initGLFW(width, height);

  initGL (window, width, height);
  glfwSetScrollCallback(window, scroll_callback);
  
  bg = Background(true);
  /* Draw in loop */
  while (!glfwWindowShouldClose(window)) {
    // Process timers
    if (t60.processTick()) {
      // 60 fps
      // OpenGL Draw commands
      draw();
      // Swap Frame Buffer in double buffering
      glfwSwapBuffers(window);

      tick_elements();
      tick_input(window);
    }
    // Poll for Keyboard and mouse events
    glfwPollEvents();
  }
  quit(window);
}

void reset_screen() {
  float top    = screen_center_y + 4 / screen_zoom;
  float bottom = screen_center_y - 4 / screen_zoom;
  float left   = screen_center_x - 4 / screen_zoom;
  float right  = screen_center_x + 4 / screen_zoom;
  Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}


void tick_input(GLFWwindow *window) {
  int left  = glfwGetKey(window, GLFW_KEY_A);
  int right = glfwGetKey(window, GLFW_KEY_D);
  int up = glfwGetKey(window, GLFW_KEY_SPACE);
  int down = glfwGetKey(window, GLFW_KEY_DOWN);

  pan_right = glfwGetKey(window, GLFW_KEY_RIGHT);
  pan_left = glfwGetKey(window, GLFW_KEY_LEFT);
  int p_auto_on = glfwGetKey(window, GLFW_KEY_G);
  int p_auto_off = glfwGetKey(window, GLFW_KEY_H);

  pan_up = glfwGetKey(window, GLFW_KEY_UP);
  pan_down = glfwGetKey(window, GLFW_KEY_DOWN);
  if( p_auto_on && !p_auto_off )
    pan_auto = true;
  else if( !p_auto_on && p_auto_off )
    pan_auto = false;
  

  for(int i=0; i<100; ++i) {
    if( ball[i].position.x > -5+eye.x && ball[i].position.x < 5+eye.x )
      if( detect_collision(player.boundary(), ball[i].boundary()) ) {
	++score;
	if( score == level1 )
	  cout<<"level 2"<<"\n";
	else if( score == level2 )
	  cout<<"level 3"<<"\n";
	glm::vec2 diff = player.position - ball[i].position;
	float sq_sum = sqrt(diff.x*diff.x + diff.y*diff.y);
	float sin_theta = diff.y / sq_sum;
	float cos_theta = diff.x / sq_sum;
	player.speed += 0.05 * cos_theta;
	player.speed_y = 0.06 * sin_theta;
	ball[i].position.x = -70;
      }
    if( i%5 == 0 && score >= level1) {
      if( detect_collision_plank( player.boundary(), bar[i/5].boundary(ball[i].position),
				  &player.speed, &player.speed_y)) {
	player.position.y += player.speed_y;
	return;
      }
    }

    if( score >= level2 )
      for(int i=0; i<2; ++i) {
	if( detect_collision_plank( player.boundary(), p[i].boundary(),
				    &player.speed, &player.speed_y)) {
	  initGL(window, 800, 600);
	}
      }
      
      
  }
  if( left && player.speed > 0 )
    player.speed -= 0.002;
  else if( right && player.speed < 0 )
    player.speed += 0.002;
  
  if( player.speed > 0.001)
    player.speed -= 0.00002;
  else if (player.speed < -0.001)
    player.speed += 0.00002;
  else
    player.speed = 0;
  //physical position of the player in the game
  type = where(player.position, player.radius, player.speed, player.speed_y);
  magnet_on = magnet.is_magnet_on(score, level3);
  
  float hyp = sqrt(pow(player.position.y + 2,2) + pow(player.position.x, 2));
  float x_diff = player.position.x;
  float y_diff = player.position.y + 2;

  float sin_t = x_diff / hyp;
  float cos_t = y_diff / hyp;

  float r = 1.3 - player.radius;
  float theta = asin(sin_t);
   
  switch(type) {
    // rules according to the location of player
  case ground:
    if( magnet_on ) {
      if( magnet.right ) {
	player.position.x += 0.015;
	player.speed = 0.005;
      }
      else {
	player.position.x -= 0.01;
	player.speed = -0.003;
      }
      player.speed_y = 0.06;
    }
    
    if( left && !right) {
      player.position.x -= 0.01;
      player.rotate_ball(-0.01);
    }
    else if( right && !left) {
      player.position.x += 0.015;
      player.rotate_ball(0.01);
    }
    if( up ) {
      player.speed_y = 0.06;
    }
    else if( player.speed_y < -0.01 )
      player.speed_y /= -1.5;
    break;

  case air:
    if( magnet_on ) {
      if( abs(player.speed) == 0 ) {
	if( magnet.right )
	  player.speed = 0.005;
	else
	  player.speed = -0.005;
      }
      
      if( magnet.right ) {
	player.speed += 0.001;
	player.position.x += player.speed;
      }
      else {
	player.speed -= 0.001;
	player.position.x += player.speed;	
      }
    }
    
    player.position.y += player.speed_y;
    player.speed_y -= gravity;
    if( left && !right) {
      player.position.x -= 0.01;
      player.rotate_ball(-0.01);
    }
    else if( right && !left) {
      player.position.x += 0.015;
      player.rotate_ball(0.01);
    }
    if( up && player.position.y < -1.6 && abs(player.position.x) > 1.3)
      player.speed_y = 0.06;
    break;

  case trampoline:
    if( magnet_on ) {
      if( magnet.right )
	player.speed += 0.0015;
      else
	player.speed -= 0.0015;
    }

    player.speed_y = 0.09;
    player.position.y = -1 + player.radius + 0.002;
    break;

  case water:
    if( w_v > 0.0005 ) {
      player.position.x -= w_v * sin(theta)/4;
      player.position.y += w_v * cos(theta);
      w_v -= 0.0005;
    }
      
    if( player.position.y < -2 ) {
      float rem =  1.3 - player.radius - sqrt(pow(player.position.y + 2,2)
					      + pow(player.position.x, 2));
      if( rem > 0.001 )
	player.position.y -= r*cos(theta) / 25;
    }
    else
      player.position.y -= 0.01;
    break;

  case w_ground:
    if( !left && !right && !magnet_on) {
      if( player.position.x > 0.001 ) {
	player.position.x = r * sin(theta - 0.009);
	player.position.y = -r * cos(theta - 0.009) - 2;
	player.rotate_ball(r * -0.01);
      }
      else if (player.position.x < 0.01 ) {
	player.position.x = r * sin(theta + 0.009);
	player.position.y = -r * cos(theta + 0.009) - 2;
	player.rotate_ball(r * 0.01);
      }
    }
    if( (left && !right) || (magnet_on && !magnet.right)) {
      	player.position.x = r * sin(theta - 0.009);
	player.position.y = -r * cos(theta - 0.009) - 2;
	player.rotate_ball(r * -0.01);
    }
    if(( right && !left)|| (magnet_on && magnet.right)) {
      	player.position.x = r * sin(theta + 0.009);
	player.position.y = -r * cos(theta + 0.009) - 2;
	player.rotate_ball(r * 0.01);
    }

    if( up ) {
      w_v = 0.05;
      player.position.x -= w_v * sin(theta);
      player.position.y += w_v * cos(theta);
    }
    
    break;

  case right_edge:
    if( right || (magnet_on && magnet.right)) {
      player.position.x = 1.3;
      player.position.y = -2 + player.radius;
    }
    else {
      player.position.x = 1.3 -player.radius-0.001;
      player.position.y = -2;
    }
    player.speed_y = 0;

    break;

  case left_edge:
    if( !left && !(magnet_on && !magnet.right)) {
      player.position.x = -1.3 +player.radius+0.001;
      player.position.y = -2;
    }
    else {
      player.position.x = -1.3;
      player.position.y = -2 + player.radius;
    }
    player.speed_y = 0;
  };	
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  if( screen_zoom + yoffset/100 > 0.5 )
    screen_zoom += yoffset/100;
  if( !pan_auto) 
    reset_screen();
  std::cout<<yoffset<<"\n";
}
