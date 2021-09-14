//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_SHOTGUN_H
#define SHOOTER_SHOTGUN_H

#include "Weapon.h"

class Shotgun : public Weapon {
public:
    explicit Shotgun(int ammo = 15, const std::string& weaponName = "shotgun");
    std::map<std::string, double> processFire(std::shared_ptr<World> world, std::shared_ptr<Camera> camera) override;
};


#endif //SHOOTER_3DZAVR_SHOTGUN_H
