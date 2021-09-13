//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AROTATE_H
#define ENGINE_AROTATE_H

#include "Animatable.h"
#include "Animation.h"

class ARotate : public Animation {
private:
    Point4D value;
public:
    ARotate(const Point4D& r, double duration, LoopOut looped, InterpolationType interpolationType) {
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update(Animatable& obj) override {
        obj.rotate(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATE_H
