//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_SHOTGUN_H
#define SHOOTER_SHOTGUN_H

#include "Weapon.h"

class Shotgun : public Weapon {
public:
    explicit Shotgun(int ammo = 15, const std::string& weaponName = "shotgun");
    std::map<std::string, double> processFire(std::function<std::pair<Point4D, std::string>(const Point4D&, const Point4D&)> rayCastFunction, const Point4D& position, const Point4D& direction) override;
};


#endif //SHOOTER_3DZAVR_SHOTGUN_H
