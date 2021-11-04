//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ASCALE_H
#define ENGINE_ASCALE_H

#include <utility>

#include "Animation.h"
#include "../physics/RigidBody.h"

class AScale final : public Animation {
private:
    const std::weak_ptr<RigidBody> _object;
    const Vec3D _scalingValue;

    void update() override {
        if (_object.expired()) {
            stop();
            return;
        }

        auto object = _object.lock();
        std::vector<Triangle> newTriangles;
        newTriangles.reserve(object->triangles().size());
        for (auto &t : object->triangles()) {
            newTriangles.emplace_back(
                    t * Matrix4x4::Scale(Vec3D{1, 1, 1} + (_scalingValue - Vec3D{1, 1, 1}) * progress()));
        }
        object.lock()->setTriangles(std::move(newTriangles));
    }

public:
    AScale(std::weak_ptr<RigidBody> object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //INC_3DZAVR_ASCALE_H
