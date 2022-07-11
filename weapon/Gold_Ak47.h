//
// Created by Иван Ильин on 05.06.2021.
//

#ifndef SHOOTER_GOLD_AK47_H
#define SHOOTER_GOLD_AK47_H

#include "Weapon.h"
#include "../ShooterConsts.h"

class Gold_Ak47 final : public Weapon {
public:
    explicit Gold_Ak47() : Weapon(200, 60, 1.5, 0.07, 50, 1.0,
                                  ShooterConsts::GOLD_AK47_FIRE_SOUND,
                                  ShooterConsts::GOLD_AK47_RELOAD_SOUND, ObjectNameTag("gold_ak47"),
                                  ShooterConsts::GOLD_AK47_OBJ, Vec3D{3, 3, 3}, Vec3D{-2.3, 0.8, 1.8},
                                  Vec3D{0, 0, 0}) {
    }
};

#endif //SHOOTER_3DZAVR_GOLD_AK47_H
