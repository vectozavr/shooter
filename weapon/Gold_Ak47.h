//
// Created by Иван Ильин on 05.06.2021.
//

#ifndef SHOOTER_GOLD_AK47_H
#define SHOOTER_GOLD_AK47_H

#include "Weapon.h"

class Gold_Ak47 : public Weapon {
public:
    explicit Gold_Ak47(int ammo = 200, const std::string& weaponName = "gold_ak47") : Weapon(weaponName, "../obj/ak47.obj", "../obj/gold_ak47_mat.txt", Point4D{3, 3, 3}, Point4D{-0.8, 1.3, 0.3}, Point4D{0, M_PI, 0}) {
        fireSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/ak47.ogg"));
        reloadSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/reload_ak47.ogg"));

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
