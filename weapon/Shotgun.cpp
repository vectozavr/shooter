//
// Created by Иван Ильин on 02.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Shotgun.h"
#include "../engine/animation/AFunction.h"
#include "../ShooterConsts.h"

using namespace std;

Shotgun::Shotgun(int ammo, const std::string& weaponName) : Weapon(weaponName, ShooterConsts::SHOTGUN_OBJ, "obj/shotgun_mat.txt", Vec3D{3, 3, 3}, Vec3D{-0.95, 1.3, -0.6}, Vec3D{0, Consts::PI, 0}) {
    fireSound = ShooterConsts::SHOTGUN_FIRE_SOUND;
    reloadSound = ShooterConsts::SHOTGUN_RELOAD_SOUND;

    //reloadSound.setVolume(30);
    _initialPack = 15;
    _clipCapacity = 1; // how much ammo can be stored in one clipx
    _stockAmmo = ammo - _clipCapacity; // how much ammo do you have in stock
    _clipAmmo = _clipCapacity; // how much ammo do you have in current clip
    _reloadTime = 1;
    _fireDelay = 1; // time delay between fires
    _damage = 400;
    _spreading = 5;
}

std::map<ObjectNameTag, double>
Shotgun::processFire(std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> rayCastFunction) {
    std::map<ObjectNameTag, double> damagedPlayers;

    for(int i = 0; i < 15; i++) {
        std::map<ObjectNameTag, double> damaged = addTrace(rayCastFunction, position() + Vec3D(triangles().back()[0]), -lookAt());
        for(auto& player : damaged)
            damagedPlayers[player.first] += player.second;
    }

    return damagedPlayers;
}
