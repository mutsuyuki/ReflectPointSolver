
#include "MyHoverCamera.hpp"


MyHoverCamera::MyHoverCamera() {
    cameraPosParam.x = PI ;
    cameraPosParam.y = 0.499 ;
    rotationSensitivity = 0.5;

    events = nullptr;
    init();

    _dragPoint = ofVec2f::zero();
}


void MyHoverCamera::update(ofEventArgs &args) {

    cameraCurrentPos->setGoal(cameraPosParam.x, cameraPosParam.y);
    cameraCurrentPos->update();
    currentDistance->update();

    ofVec3f orbitPos = getOrbitPosition(cameraCurrentPos->current, currentDistance->current);
    this->setPosition(orbitPos.x, orbitPos.y, orbitPos.z);

    cameraPosParamTemp = cameraPosParam;

    this->lookAt(target);
}


void MyHoverCamera::addDistance(float delta) {

    this->distance = this->distance * ( 1 + (delta - 1) * this->distanceSensitivity);
    this->distance = calcDistance( distance );
    currentDistance->setGoal( distance );
    cameraCurrentPos->update();
}


float MyHoverCamera::calcDistance(float __distance) {

    float target = __distance;
    if ( target < minDistance )
    {
        target = minDistance;
    }
    if ( target > maxDistance )
    {
        target = maxDistance;
    }
    return target;
}

void MyHoverCamera::onEnable() {

    ofLog(OF_LOG_NOTICE, (string) typeid(*this).name() + " : " + __FUNCTION__ + "");
    if (!isTangibleSeted && events) {
        ofAddListener(events->update, this, &MyHoverCamera::update);
        ofAddListener(events->mouseDragged, this, &MyHoverCamera::mouseDragged);
        ofAddListener(events->mousePressed, this, &MyHoverCamera::mousePressed);
        ofAddListener(events->mouseReleased, this, &MyHoverCamera::mouseReleased);
        ofAddListener(events->mouseScrolled, this, &MyHoverCamera::mouseScrolled);
    }

    isTangibleSeted = true;
}


void MyHoverCamera::onDisable() {
    ofLog(OF_LOG_NOTICE, (string) typeid(*this).name() + " : " + __FUNCTION__ + "");
    if (isTangibleSeted && events) {
        ofRemoveListener(events->update, this, &MyHoverCamera::update);
        ofRemoveListener(events->mouseDragged, this, &MyHoverCamera::mouseDragged);
        ofRemoveListener(events->mousePressed, this, &MyHoverCamera::mousePressed);
        ofRemoveListener(events->mouseReleased, this, &MyHoverCamera::mouseReleased);
        ofRemoveListener(events->mouseScrolled, this, &MyHoverCamera::mouseScrolled);
    }

    isTangibleSeted = false;
    isDragging = false;
}

void MyHoverCamera::mouseMoved(ofMouseEventArgs &mouse) {

}


void MyHoverCamera::mouseDragged(ofMouseEventArgs &mouse) {


    if(!isDragging)
        return;

    ofVec2f currentDragPoint = ofVec2f(events->getMouseX(), events->getMouseY());

    ofVec2f diff = currentDragPoint - _dragPoint;

    float length = diff.length();
    diff.normalize();
    diff.scale(length * 0.003);

    this->cameraPosParam.x = this->cameraPosParamTemp.x - diff.x * 1.5;
    this->cameraPosParam.y = this->cameraPosParamTemp.y + diff.y;

    if (cameraPosParam.y < this->minRotationY * PI) {
        cameraPosParam.y = this->minRotationY * PI;
    }
    else if (cameraPosParam.y > this->maxRotationY * PI) {
        cameraPosParam.y = this->maxRotationY * PI;
    }

    _dragPoint = currentDragPoint;

    int id = 0;
    ofNotifyEvent(onDragEvent, id);
}


void MyHoverCamera::mousePressed(ofMouseEventArgs &mouse) {
    _dragPoint = ofVec2f(events->getMouseX(), events->getMouseY());

    if(dragArea.getArea() == 0 ||  dragArea.inside(mouse))
    {
        isDragging = true;
    }
}


void MyHoverCamera::mouseReleased(ofMouseEventArgs &mouse) {
    isDragging = false;
}


void MyHoverCamera::mouseScrolled(ofMouseEventArgs &mouse) {
    addDistance( mouse.scrollY / 10 + 1 );
}

void MyHoverCamera::mouseEntered(ofMouseEventArgs &mouse) {

}


void MyHoverCamera::mouseExited(ofMouseEventArgs &mouse) {
    isDragging = false;
}

void MyHoverCamera::begin(ofRectangle __viewport) {
    if (!isEventSeted) {
        setEvents(ofEvents());
    }

    viewport = getViewport(__viewport);
    ofCamera::begin(viewport);
}

void MyHoverCamera::end() {
    ofCamera::end();
}


void MyHoverCamera::setEvents(ofCoreEvents &_events) {
    onDisable();
    events = &_events;
    if (tangible) {
        // note: this will set bMouseInputEnabled to true as a side-effect.
        onEnable();
    }
    isEventSeted = true;
}


void MyHoverCamera::setTangible(bool __bool) {

    if(this->tangible == __bool)
        return;

    this->tangible = __bool;
    if(this->tangible)
    {
        this->onEnable();
    } else{
        this->onDisable();
    }

    isEventSeted = true;
}
