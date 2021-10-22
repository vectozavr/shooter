//
// Created by Иван Ильин on 02.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Ak47.h"
#include "../ShooterConsts.h"

using namespace std;

Ak47::Ak47(int ammo, const std::string& weaponName) : Weapon(weaponName, ShooterConsts::AK47_OBJ, Vec3D{3, 3, 3}, Vec3D{-2.2, 1.0, 1.3}, Vec3D{0, Consts::PI, 0}) {
    fireSound = ShooterConsts::AK47_FIRE_SOUND;
    reloadSound = ShooterConsts::AK47_RELOAD_SOUND;


    _stockAmmo = ammo - _clipCapacity;
    _fireDelay = 0.1;
}
