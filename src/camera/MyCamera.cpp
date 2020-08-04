
#include "MyCamera.hpp"
#include "SmoothFloat.hpp"
#include "Tweenzor.h"

MyCamera::MyCamera() {
    currentDistance = new SmoothFloat();
    cameraCurrentPos = new SmoothPoint();
}

void MyCamera::init() {

    cameraCurrentPos = new SmoothPoint();
    cameraCurrentPos->reset(cameraPosParam.x, cameraPosParam.y);

    currentDistance = new SmoothFloat();
    currentDistance->reset(distance);
    currentDistance->sensitivity = 0.05;

    ofVec3f orbitPos = getOrbitPosition(cameraCurrentPos->current, currentDistance->current);
    ofVec3f pivot = target;

    this->setPosition(pivot.x + orbitPos.x, pivot.y + orbitPos.y, pivot.z + orbitPos.z);

    this->lookAt(target);
    this->cameraPosParamTemp = this->cameraPosParam;
}

ofVec3f MyCamera::getOrbitPosition(ofVec2f __point, float __distance) {

    ofVec3f calPosition = ofVec3f();
    calPosition.x = sin(__point.x) * cos(__point.y);
    calPosition.z = cos(__point.x) * cos(__point.y);
    calPosition.y = sin(__point.y);

    calPosition.normalize();
    calPosition.scale(__distance);
    return calPosition;
}

void MyCamera::customDraw() {
    ofNode::customDraw();

    ofDrawBox(this->getX(), this->getY(), this->getZ(), 20);

    ofSetColor(255, 0, 0);
    ofDrawBox(target, 40);
    ofSetColor(255);

    ofDrawLine(getPosition(), target);
}

void MyCamera::moveTo(ofVec2f __camera) {
    cancelMove();
    Tweenzor::add(&cameraPosParam, cameraPosParam, __camera, 0, 1.0f, EASE_IN_OUT_SINE);
}

void MyCamera::moveTo(ofVec2f __camera, ofVec3f __target) {

    cancelMove();
    Tweenzor::add(&target, target, __target, 0, 1.0f, EASE_IN_OUT_SINE);
    Tweenzor::add(&cameraPosParam, cameraPosParam, __camera, 0, 1.0f, EASE_IN_OUT_SINE);
}

void MyCamera::cancelMove() {

    if (Tweenzor::getTween(&target))
        Tweenzor::getTween(&target)->pause();

    if (Tweenzor::getTween(&cameraPosParam))
        Tweenzor::getTween(&cameraPosParam)->pause();
}
