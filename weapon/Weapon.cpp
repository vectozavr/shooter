//
// Created by Иван Ильин on 01.06.2021.
//

#include <sstream>
#include <utility>
#include "Weapon.h"
#include "../engine/ResourceManager.h"
#include "../engine/utils/Log.h"
#include "../engine/animation/AColor.h"
#include "../engine/animation/AFunction.h"

using namespace std;

Weapon::Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Vec3D& scale, const Vec3D& t, const Vec3D& r) {
    _name = weaponName;

    loadObj(objFileName, matFileName, scale);
    setCollider(false);
    rotate(r);
    translate(t);

    noAmmoSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/no_ammo.ogg"));
}

std::map<std::string, double> Weapon::fire(std::function<std::pair<Vec3D, std::string>(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& position, const Vec3D& direction) {
    if(_clipAmmo == 0) {
        reload();
        if(_clipAmmo == 0)
            noAmmoSound.play();
    }

    if(_clipAmmo <= 0 || std::abs(Time::time() - _lastFireTime) < _fireDelay || std::abs(Time::time() - _lastReloadTime) < _reloadTime)
        return std::map<std::string, double>();

    _lastFireTime = Time::time();
    _clipAmmo--;

    fireSound.play();
    Log::log("Weapon::fire (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");

    return processFire(std::move(rayCastFunction), position, direction);
}

void Weapon::reload() {
    if (_stockAmmo == 0 || std::abs(Time::time() - _lastReloadTime) < _reloadTime)
        return;
    if(_clipCapacity - _clipAmmo <= _stockAmmo) {
        _stockAmmo -= _clipCapacity - _clipAmmo;
        _clipAmmo = _clipCapacity;
    } else {
        _clipAmmo += _stockAmmo;
        _stockAmmo = 0;
    }

    reloadSound.play();
    Log::log("Weapon::reload (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");
    _lastReloadTime = Time::time();
}

std::map<std::string, double> Weapon::processFire(std::function<std::pair<Vec3D, std::string>(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& pos, const Vec3D& direction) {
    std::map<std::string, double> damagedPlayers;

    //generate random vector
    Vec3D randV(10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

    // damage player
    auto rayCast = rayCastFunction(pos, pos + direction * 1000 + randV);
    if(rayCast.second.find("Player") != std::string::npos) {
        damagedPlayers[rayCast.second] += _damage/(1.0 + (pos - rayCast.first).abs());
    }

    // add trace line
    Vec3D to = rayCast.first == Vec3D(0) ? pos + direction * 1000 + randV: rayCast.first;
    Vec3D from = position() + Vec3D(triangles().back()[0]);
    _addTraceCallBack(from, to);

    return damagedPlayers;
}
