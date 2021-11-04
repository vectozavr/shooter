//
// Created by Иван Ильин on 04.11.2021.
//

#include "HitBox.h"
#include "../Consts.h"

HitBox::HitBox(const Mesh &mesh) {
    for(const auto& t : mesh.triangles()) {
        for(int i = 0; i < 3; i++) {
            // we dont need to add the same points in hit box
            _addIfUnique(Vec3D(t[i]));
        }
    }
}

void HitBox::_addIfUnique(const Vec3D &point) {
    bool addPoint = true;
    for(const auto& p : _hitBox) {
        if(p == point) {
            addPoint = false;
        }
    }
    if(addPoint) {
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
