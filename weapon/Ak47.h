//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_AK47_H
#define SHOOTER_AK47_H

#include "Weapon.h"
#include "../ShooterConsts.h"

class Ak47 final : public Weapon {
public:
    explicit Ak47() : Weapon(100, 30, 3.0, 0.1, 50, 2.0,
                             ShooterConsts::AK47_FIRE_SOUND, ShooterConsts::AK47_RELOAD_SOUND,
                             ObjectNameTag("ak47"), ShooterConsts::AK47_OBJ,
                             Vec3D{3, 3, 3}, Vec3D{-2.3, 0.8, 1.8},Vec3D{0, 0, 0}) {}
};

#endif //SHOOTER_3DZAVR_AK47_H
