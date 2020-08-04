
#include "SmoothPoint.hpp"


SmoothPoint::SmoothPoint() {
    prev = ofVec2f();
    current = ofVec2f();

}

void SmoothPoint::update() {
    prev = current;
    current.x = current.x + (target.x - current.x) * sensitivity;
    current.y = current.y + (target.y - current.y) * sensitivity;
}

void SmoothPoint::setGoal(float __x, float __y) {
    target.x = __x;
    target.y = __y;
}

void SmoothPoint::reset(float __x, float __y) {
    target.x = current.x = prev.x = __x;
    target.y = current.y = prev.y = __y;
}
