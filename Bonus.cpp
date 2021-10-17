//
// Created by Иван Ильин on 05.06.2021.
//

#include "Bonus.h"

Bonus::Bonus(const std::string &bonusName, const std::string &filename, const Vec3D &scale) {
    _name = bonusName;
    loadObj(filename, scale);
    setCollider(false);
}
