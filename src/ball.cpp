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
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    rotate = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    GLfloat vertex_buffer_data[] = {
      0.0, 0.0, 0,
      1.0, 0.0, 0,
      0.0, 1.0, 0,
    }, theta = 0, r = this->radius, x, y;;
    color_t n = COLOR_BLACK, m = this->color, temp;    
    GLuint num_segments = 24;
    
    for(int i = 0; i <= num_segments; i+=2)
      {
	compute_next_vertex(i, num_segments, vertex_buffer_data, &theta, this->radius);
	
	object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, m, GL_FILL);
	draw3DObject(object);

	if( dual) {
	  temp = m;
	  m = n;
	  n = temp;
	}
      }
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= this->speed;
    this->position.y += this->speed_y;
    if(( this->position.y != -2 || this->speed_y == 0.04 ))
    this->speed_y -= 0.001;
    if( this->position.y < -2) {
      this->speed_y = 0;
      this->position.y = -2;
    }
      
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

void Ball::rotate_ball(float theta) {
  this->rotation += theta;
}
