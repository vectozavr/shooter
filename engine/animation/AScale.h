//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ASCALE_H
#define ENGINE_ASCALE_H

#include "Animatable.h"
#include "Animation.h"
#include "Mesh.h"

class AScale : public Animation {
private:
    std::shared_ptr<Mesh> _mesh;

    Point4D value;
    std::vector<Triangle> triangles{};
public:
    AScale(std::shared_ptr<Mesh> mesh, const Point4D &s, double duration = 1, LoopOut looped = LoopOut::None, InterpolationType interpolationType = InterpolationType::bezier) {
        _mesh = mesh;
        _duration = duration;
        _looped = looped;
        _intType = interpolationType;
        _waitFor = true;

        value = s;
    }

    bool update() override {
        if(!_started)
            triangles = _mesh->triangles();

        std::vector<Triangle> newTriangles;
        for(auto &t : triangles) {
            newTriangles.emplace_back(t * Matrix4x4::Scale(Point4D{1, 1, 1} + (value - Point4D{1, 1, 1}) * _p));
        }
        _mesh->setTriangles(newTriangles);
        return updateState();
    }
};

#endif //INC_3DZAVR_ASCALE_H
