//
// Created by Иван Ильин on 02.06.2021.
//

#include "../engine/ResourceManager.h"
#include "Shotgun.h"
#include "../engine/animation/AColor.h"
#include "../engine/animation/AFunction.h"

using namespace std;

Shotgun::Shotgun(int ammo, const std::string& weaponName) : Weapon(weaponName, "obj/shotgun.obj", "obj/shotgun_mat.txt", Vec3D{3, 3, 3}, Vec3D{-0.95, 1.3, -0.6}, Vec3D{0, Consts::PI, 0}) {
    fireSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/shotgun.ogg"));
    reloadSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/reload_shotgun.ogg"));

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

std::map<std::string, double>
Shotgun::processFire(std::function<std::pair<Vec3D, std::string>(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& pos, const Vec3D& direction) {
    std::map<std::string, double> damagedPlayers;

    for(int i = 0; i < 15; i++) {
        //generate random vector
        Vec3D randV(10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

        // damage player
        auto rayCast = rayCastFunction(pos, pos + direction * 1000 + randV);
        if (rayCast.second.find("Enemy") != std::string::npos) {
            damagedPlayers[rayCast.second] += _damage / (1.0 + (pos - rayCast.first).abs());
        }

        Vec3D to = rayCast.first == Vec3D(0) ? pos + direction * 1000 + randV: rayCast.first;
        Vec3D from = position() + Vec3D(triangles().back()[0]);
        _addTraceCallBack(from, to);
    }


    return damagedPlayers;
}
