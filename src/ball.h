#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float r, color_t c, bool dual);
    glm::vec3 position;
    float radius;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void rotate_ball(float disp);
    Boundary boundary();
    //    void compute_next_vertex(int i, int n, float vbd[], float* theta);
    void tick();
    double speed;
    double speed_y;
    color_t color;
    bool dual;
    //    bounding_box_t bounding_box();
private:
    VAO *object, *o1, *o2, *o3, *o4;
};

#endif // BALL_H
