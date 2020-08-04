
#include "SmoothFloat.hpp"

SmoothFloat::SmoothFloat() {

}

void SmoothFloat::update() {
    if (current != target) {
        current = current + (target - current) * sensitivity;
    }
}

void SmoothFloat::setGoal(float __value) {
    target = __value;
}


void SmoothFloat::reset(float __value) {
    current = target = __value;
}

