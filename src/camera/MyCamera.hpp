
#ifndef MYCAMERA_HPP
#define MYCAMERA_HPP

#include "ofMain.h"
#include "SmoothPoint.hpp"
#include "SmoothFloat.hpp"

class MyCamera : public ofCamera {

public:

    MyCamera();

    ofVec3f target = ofVec3f();
    float distanceSensitivity = 0.2;
    float rotationSensitivity = 0.5;

    float maxRotationY = 0.499999;
    float minRotationY = -0.45;

    bool isRestrictX = false;
    float maxRotationX = 0.45;
    float minRotationX = -0.45;
    float minDistance = 10;
    float maxDistance = 20000;
    float distance = 300;

    SmoothFloat *currentDistance;
    ofVec2f cameraPosParam = ofVec2f();
    ofVec2f cameraPosParamTemp = ofVec2f();
    SmoothPoint *cameraCurrentPos;

    virtual void customDraw() override;

    void moveTo(ofVec2f __camera);
    void moveTo(ofVec2f __camera, ofVec3f __target);

    void init();

    void cancelMove();

protected:
    ofVec3f getOrbitPosition(ofVec2f __point, float __distance);
};


#endif //MYCAMERA_HPP
