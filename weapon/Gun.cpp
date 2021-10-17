//
// Created by Иван Ильин on 03.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Gun.h"
#include "../ShooterConsts.h"

using namespace std;

Gun::Gun(int ammo, const std::string& weaponName) : Weapon(weaponName, ShooterConsts::GUN_OBJ, "obj/gun_mat.txt", Vec3D{3, 3, 3}, Vec3D{-0.8, 1.3, 0.3}, Vec3D{0, Consts::PI, 0}) {
    fireSound = ShooterConsts::GUN_FIRE_SOUND;
    reloadSound = ShooterConsts::GUN_RELOAD_SOUND;

    _initialPack = 30;
    _clipCapacity = 6; // how much ammo can be stored in one clip
    _stockAmmo = ammo - _clipCapacity; // how much ammo do you have in stock
    _clipAmmo = _clipCapacity; // how much ammo do you have in current clip
    _reloadTime = 2;
    _fireDelay = 0.3; // time delay between fires
    _damage = 800;
    _spreading = 3.0;
}
