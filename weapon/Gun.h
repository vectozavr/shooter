//
// Created by Иван Ильин on 03.06.2021.
//

#ifndef SHOOTER_GUN_H
#define SHOOTER_GUN_H

#include "Weapon.h"

class Gun final : public Weapon {
public:
    explicit Gun(int ammo = 30, const std::string& weaponName = "gun");
};


#endif //SHOOTER_3DZAVR_GUN_H
