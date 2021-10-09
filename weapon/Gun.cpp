//
// Created by Иван Ильин on 03.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Gun.h"

using namespace std;

Gun::Gun(int ammo, const std::string& weaponName) : Weapon(weaponName, "obj/gun.obj", "obj/gun_mat.txt", Point4D{3, 3, 3}, Point4D{-0.8, 1.3, 0.3}, Point4D{0, Consts::PI, 0}) {
    fireSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/gun.ogg"));
    reloadSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/reload_gun.ogg"));

    _initialPack = 30;
    _clipCapacity = 6; // how much ammo can be stored in one clip
    _stockAmmo = ammo - _clipCapacity; // how much ammo do you have in stock
    _clipAmmo = _clipCapacity; // how much ammo do you have in current clip
    _reloadTime = 2;
    _fireDelay = 0.3; // time delay between fires
    _damage = 800;
    _spreading = 3.0;
}
