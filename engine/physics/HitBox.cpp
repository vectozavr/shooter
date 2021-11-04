//
// Created by Иван Ильин on 04.11.2021.
//

#include <algorithm>
#include <execution>

#include "HitBox.h"
#include "../Consts.h"

HitBox::HitBox(const Mesh &mesh) {
    _hitBox.reserve(mesh.triangles().size() * 3);
    for(const auto& t : mesh.triangles()) {
        for(int i = 0; i < 3; i++) {
            // we dont need to add the same points in hit box
            _addIfUnique(Vec3D(t[i]));
        }
    }
    _hitBox.shrink_to_fit();
}

void HitBox::_addIfUnique(Vec3D &&point) {

    auto check = [&point](const auto& p) { return p == point; };

    if (std::find_if(std::execution::par, _hitBox.rbegin(), _hitBox.rend(), check) == _hitBox.rend()) {
        _hitBox.push_back(point);
    }
}

HitBox HitBox::Box(const Mesh &mesh) {
    HitBox result;

    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();
    double maxZ = -std::numeric_limits<double>::max();

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();

    for(const auto& t : mesh.triangles()) {
        for(int i = 0; i < 3; i++) {
            Vec3D point = Vec3D(t[i]);
            if(point.x() > maxX) {
                maxX = point.x();
            }
            if(point.y() > maxY) {
                maxY = point.y();
            }
            if(point.z() > maxZ) {
                maxZ = point.z();
            }

            if(point.x() < minX) {
                minX = point.x();
            }
            if(point.y() < minY) {
                minY = point.y();
            }
            if(point.z() < minZ) {
                minZ = point.z();
            }
        }
    }

    result._hitBox.emplace_back(minX, minY, minZ);
    result._hitBox.emplace_back(minX, maxY, minZ);
    result._hitBox.emplace_back(maxX, minY, minZ);
    result._hitBox.emplace_back(maxX, maxY, minZ);

    result._hitBox.emplace_back(minX, minY, maxZ);
    result._hitBox.emplace_back(minX, maxY, maxZ);
    result._hitBox.emplace_back(maxX, minY, maxZ);
    result._hitBox.emplace_back(maxX, maxY, maxZ);

    return result;
}
