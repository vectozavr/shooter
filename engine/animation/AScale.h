//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ASCALE_H
#define ENGINE_ASCALE_H

#include "Animation.h"
#include "../physics/RigidBody.h"

class AScale final : public Animation {
private:
    const std::weak_ptr<RigidBody> _object;
    const Vec3D _scalingValue;

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }

        std::vector<Triangle> newTriangles;
        newTriangles.reserve(obj->triangles().size());
        for (auto &t : obj->triangles()) {
            newTriangles.emplace_back(t * Matrix4x4::Scale(Vec3D{1, 1, 1} +
                                    (_scalingValue - Vec3D{1, 1, 1}) * progress())
                                    );
        }
        obj->setTriangles(std::move(newTriangles));
    }

public:
    AScale(std::weak_ptr<RigidBody> object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //INC_3DZAVR_ASCALE_H
