
#ifndef SMOOTHFLOAT_HPP
#define SMOOTHFLOAT_HPP


class SmoothFloat {

public:
    SmoothFloat();

    float current = 0;
    float target = 0;
    float sensitivity = 0.03f;

    void update();

    void setGoal(float __value);

    void reset(float __value);
};


#endif //SMOOTHFLOAT_HPP
