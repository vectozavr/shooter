//
// Created by Иван Ильин on 06.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Rifle.h"

Rifle::Rifle(int ammo, const std::string &weaponName) : Weapon(weaponName, "obj/rifle.obj", "obj/rifle_mat.txt", Vec3D{3, 3, 3}, Vec3D{-1.2, 1, 0.3}, Vec3D{0, Consts::PI, 0}) {
    fireSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/shotgun.ogg"));
    reloadSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/reload_ak47.ogg"));

    _initialPack = 5;
    _clipCapacity = 1; // how much ammo can be stored in one clip
    _stockAmmo = ammo - _clipCapacity; // how much ammo do you have in stock
    _clipAmmo = _clipCapacity; // how much ammo do you have in current clip
    _reloadTime = 1;
    _fireDelay = 1; // time delay between fires
    _damage = 30000;
    _spreading = 0.5;
}
