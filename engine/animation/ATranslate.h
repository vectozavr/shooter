//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ATRANSLATE_H
#define ENGINE_ATRANSLATE_H

#include "Animation.h"
#include "../Object.h"

class ATranslate : public Animation {
private:
    std::shared_ptr<Object> _object;

    Vec3D value;
public:
    ATranslate(std::shared_ptr<Object> object, const Vec3D& t, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::bezier) : value(t){
        _object = object;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
    }

    bool update() override {
        _object->translate(value * _dp);
        return updateState();
    }
};

#endif //INC_3DZAVR_ATRANSLATE_H
