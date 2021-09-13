//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_AK47_H
#define SHOOTER_AK47_H

#include "Weapon.h"

class Ak47 : public Weapon {
public:
    explicit Ak47(int ammo = 100, const std::string& weaponName = "ak47");
};


#endif //SHOOTER_3DZAVR_AK47_H
