//
// Created by Иван Ильин on 06.06.2021.
//

#ifndef SHOOTER_RIFLE_H
#define SHOOTER_RIFLE_H

#include "Weapon.h"

class Rifle : public Weapon {
public:
    explicit Rifle(int ammo = 5, const std::string& weaponName = "rifle");
};


#endif //SHOOTER_3DZAVR_RIFLE_H
