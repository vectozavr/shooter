//
// Created by Иван Ильин on 10.11.2021.
//

#ifndef SHOOTER_ASHOWCREATION_H
#define SHOOTER_ASHOWCREATION_H

#include "Animation.h"
#include "../Mesh.h"

class AShowCreation final : public Animation {
private:
    const std::weak_ptr<Mesh> _mesh;
    const std::vector<Triangle> _triangles;

    void update() override {
        auto mesh = _mesh.lock();

        if (mesh == nullptr) {
            stop();
            return;
        }

        std::vector<Triangle> newTriangles;
        newTriangles.reserve(_triangles.size());
        for(auto &t : _triangles) {
            newTriangles.emplace_back(t[0], t[1], t[1] + (t[2] - t[1]) * progress(), t.color());
        }
        mesh->setTriangles(std::move(newTriangles));
        mesh->glFreeFloatArray();
    }

public:
    AShowCreation(std::weak_ptr<Mesh> mesh, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _mesh(mesh), _triangles(mesh.lock()->triangles()) {}
};

#endif //SHOOTER_ASHOWCREATION_H
