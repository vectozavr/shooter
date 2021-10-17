//
// Created by Иван Ильин on 05.06.2021.
//

#ifndef SHOOTER_GOLD_AK47_H
#define SHOOTER_GOLD_AK47_H

#include "Weapon.h"
#include "../ShooterConsts.h"

class Gold_Ak47 final : public Weapon {
public:
    explicit Gold_Ak47(int ammo = 200, const std::string& weaponName = "gold_ak47") : Weapon(weaponName, ShooterConsts::GOLD_AK47_OBJ, "obj/gold_ak47_mat.txt", Vec3D{3, 3, 3}, Vec3D{-0.8, 1.3, 0.3}, Vec3D{0, Consts::PI, 0}) {
        fireSound = ShooterConsts::GOLD_AK47_FIRE_SOUND;
        reloadSound = ShooterConsts::GOLD_AK47_RELOAD_SOUND;

        _initialPack = 200;
        _spreading = 1.0;
        _reloadTime = 1.5;
        _clipCapacity = 60;
        _stockAmmo = ammo - _clipCapacity;
        _fireDelay = 0.05;
        _damage = 600;
        _clipAmmo = _clipCapacity;
    }
};

#endif //SHOOTER_3DZAVR_GOLD_AK47_H
