//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ATRANSLATE_H
#define ENGINE_ATRANSLATE_H

#include <utility>

#include "Animation.h"
#include "../Object.h"

class ATranslate final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _translationValue;

    void update() override {
        if(_object.expired()) {
            stop();
            return;
        }

        _object.lock()->translate(_translationValue * dprogress());
    }
public:
    ATranslate(std::weak_ptr<Object> object, const Vec3D& t, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped, interpolationType), _object(std::move(object)), _translationValue(t){
    }
};

#endif //INC_3DZAVR_ATRANSLATE_H
