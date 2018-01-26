#include "main.h"
#include "timer.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
int collided = 0;

Ball ball1, ball2;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
Timer t60(1.0 / 60);

void draw() {
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram (programID);

  // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
  Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

  // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
  glm::mat4 VP = Matrices.projection * Matrices.view;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  // Don't change unless you are sure!!
  glm::mat4 MVP;  // MVP = Projection * View * Model

  // Scene render
  ball1.draw(VP);
  ball2.draw(VP);
}



void tick_input(GLFWwindow *window) {
  int left  = glfwGetKey(window, GLFW_KEY_LEFT);
  int right = glfwGetKey(window, GLFW_KEY_RIGHT);
  int up = glfwGetKey(window, GLFW_KEY_UP);
  int down = glfwGetKey(window, GLFW_KEY_DOWN);
  int coll = detect_collision(ball1.boundary(), ball2.boundary());
  int right_collided = 0, left_collided = 0;
  if( coll )
    ball1.speed_y = 0;
  if( coll && (ball1.position.y == ball2.position.y) ) {
    if( ball1.position.x < ball2.position.x )
      left_collided = 1;
    else
      right_collided = 1;
  }
  else
    left_collided = right_collided = 0;
  if (left && !collided && !right_collided) {
    ball1.rotate_ball(2.0);
    ball1.position.x -= 0.01;
  }
  if (right && !collided && !left_collided) {
    ball1.rotate_ball(-2.0);
    ball1.position.x += 0.01;
  }
  if( up && ball1.speed_y == 0) {
    ball1.speed_y = 0.06;
  }
}

void tick_elements() {
  ball1.tick();
  ball2.tick();
  if (detect_collision(ball1.boundary(), ball2.boundary())) {
    ball1.speed = 0;
    //      collided = 1;
    if( ball1.position.y > ball2.position.y )
      ball1.position.y = ball2.position.y + 0.4;
    ball1.rotate_ball(-ball1.rotation);
    //        ball2.speed = -ball2.speed;
  }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
  /* Objects should be created before any other gl function and shaders */
  // Create the models

  ball1       = Ball(2, -2, 0.3, COLOR_RED, true);
  ball2       = Ball(-2, -2, 0.4, COLOR_RED, false);
  ball2.speed = 0;
  ball1.speed = 0;

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
  int width  = 600;
  int height = 600;

  window = initGLFW(width, height);

  initGL (window, width, height);

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
