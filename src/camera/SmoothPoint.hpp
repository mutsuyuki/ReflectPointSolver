
#ifndef SMOOTHPOINT_HPP
#define SMOOTHPOINT_HPP

#include "ofMain.h"

class SmoothPoint {

public:
    SmoothPoint();

    ofVec2f prev;
    ofVec2f current;
    ofVec2f target;
    float sensitivity = 0.15f;

    void update();

    void reset(float __x, float __y);

    void setGoal(float __x, float __y);
};


#endif //SMOOTHPOINT_HPP
