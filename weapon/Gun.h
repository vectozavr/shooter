//
// Created by Иван Ильин on 03.06.2021.
//

#ifndef SHOOTER_GUN_H
#define SHOOTER_GUN_H

#include "Weapon.h"
#include "../engine/utils/ResourceManager.h"
#include "../ShooterConsts.h"

class Gun final : public Weapon {
public:
    explicit Gun() : Weapon(30, 6, 2.0, 0.3, 150, 3.0,
                            ShooterConsts::GUN_FIRE_SOUND, ShooterConsts::GUN_RELOAD_SOUND, ObjectNameTag("gun"),
                            ShooterConsts::GUN_OBJ, Vec3D{3, 3, 3},
                            Vec3D{-2.2, 1.0, 2.0}, Vec3D{0, 0, 0}) {}
};


#endif //SHOOTER_3DZAVR_GUN_H
