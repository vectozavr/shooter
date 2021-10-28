//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_SHOTGUN_H
#define SHOOTER_SHOTGUN_H

#include "Weapon.h"

class Shotgun final : public Weapon {
public:
    explicit Shotgun(const std::string& weaponName = "shotgun");
    std::map<ObjectNameTag, double> processFire(std::function<IntersectionInformation(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& position, const Vec3D& direction) override;
};


#endif //SHOOTER_3DZAVR_SHOTGUN_H
