//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_AROTATE_H
#define ENGINE_AROTATE_H

#include "Animation.h"
#include "Object.h"

class ARotate : public Animation {
private:
    std::shared_ptr<Object> _object;

    Point4D value;
public:
    ARotate(std::shared_ptr<Object> object, const Point4D& r, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::bezier) {
        _object = object;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;

        value = r;
    }

    bool update() override {
        _object->rotate(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_AROTATE_H
