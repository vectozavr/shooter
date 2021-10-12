//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ATRANSLATETOPOINT_H
#define ENGINE_ATRANSLATETOPOINT_H

#include "Animation.h"
#include "../Object.h"

class ATranslateToPoint : public Animation {
private:
    std::shared_ptr<Object> _object;

    Vec3D point;
    std::unique_ptr<Vec3D> value;
public:
    ATranslateToPoint(std::shared_ptr<Object> object, const Vec3D& p, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::bezier) : point(p) {
        _object = object;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
    }

    bool update() override {
        if(!_started) {
            value = std::make_unique<Vec3D>(point - _object->position());
        }
        _object->translate(*value * _dp);

        return updateState();
    }
};

#endif //INC_3DZAVR_ATRANSLATETOPOINT_H
