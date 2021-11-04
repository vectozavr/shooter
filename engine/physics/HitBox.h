//
// Created by Иван Ильин on 04.11.2021.
//

#ifndef SHOOTER_HITBOX_H
#define SHOOTER_HITBOX_H

#include "../Mesh.h"

class HitBox final {
private:
    std::vector<Vec3D> _hitBox;

    void _addIfUnique(Vec3D &&point);

public:
    HitBox() = default;

    explicit HitBox(const Mesh &mesh);

    [[nodiscard]] std::vector<Vec3D>::iterator begin() { return _hitBox.begin(); }

    [[nodiscard]] std::vector<Vec3D>::iterator end() { return _hitBox.end(); }

    HitBox static Box(const Mesh &mesh);
};


#endif //SHOOTER_HITBOX_H
