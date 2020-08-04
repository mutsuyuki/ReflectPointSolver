
#ifndef MYHOVERCAMERA_HPP
#define MYHOVERCAMERA_HPP


#include "ofMain.h"
#include "MyCamera.hpp"

class MyHoverCamera : public MyCamera {


public:

    ofEvent<int> onDragEvent;

    MyHoverCamera();

    MyHoverCamera(bool tangible);

    virtual void begin(ofRectangle viewport) override;

    virtual void end() override;

    bool tangible = true;

    void setTangible(bool __bool);

    bool isDragging = false;
    ofRectangle dragArea;

    void update(ofEventArgs &args);

    // for mouse
    void mouseMoved(ofMouseEventArgs &mouse);

    void mouseDragged(ofMouseEventArgs &mouse);

    void mousePressed(ofMouseEventArgs &mouse);

    void mouseReleased(ofMouseEventArgs &mouse);

    void mouseScrolled(ofMouseEventArgs &mouse);

    void mouseEntered(ofMouseEventArgs &mouse);

    void mouseExited(ofMouseEventArgs &mouse);

protected:
    ofVec2f _dragPoint;

    ofRectangle viewport;
    ofVec2f mouse;

    bool isEventSeted = false;
    bool isTangibleSeted = false;

    ofCoreEvents *events;

    void setEvents(ofCoreEvents &_events);

    void onEnable();

    void onDisable();

    void addDistance(float delta);

    float calcDistance(float __distance);
};


#endif //MYHOVERCAMERA_HPP
