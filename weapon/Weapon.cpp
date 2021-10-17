//
// Created by Иван Ильин on 01.06.2021.
//

#include <utility>
#include "Weapon.h"
#include "../engine/ResourceManager.h"
#include "../engine/utils/Log.h"
#include "../engine/animation/AColor.h"
#include "../engine/animation/AFunction.h"
#include "../ShooterConsts.h"

using namespace std;

Weapon::Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Vec3D& scale, const Vec3D& t, const Vec3D& r) {
    _name = weaponName;

    loadObj(objFileName, matFileName, scale);
    setCollider(false);
    rotate(r);
    translate(t);
}

std::map<ObjectNameTag, double> Weapon::fire(std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> rayCastFunction) {
    if(_clipAmmo == 0) {
        reload();
        if(_clipAmmo == 0)
            SoundController::playSound(SoundTag("noAmmo"), ShooterConsts::NO_AMMO_SOUND);
    }

    if(_clipAmmo <= 0 || std::abs(Time::time() - _lastFireTime) < _fireDelay || std::abs(Time::time() - _lastReloadTime) < _reloadTime)
        return std::map<ObjectNameTag, double>();

    _lastFireTime = Time::time();
    _clipAmmo--;

    SoundController::playSound(SoundTag("fire"), fireSound);
    Log::log("Weapon::fire (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");

    return processFire(std::move(rayCastFunction));
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

    SoundController::playSound(SoundTag("fire"), reloadSound);
    Log::log("Weapon::reload (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");
    _lastReloadTime = Time::time();
}

std::map<ObjectNameTag, double> Weapon::processFire(std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> rayCastFunction) {
    return addTrace(std::move(rayCastFunction), position() + Vec3D(triangles().back()[0]), -lookAt());
}

std::map<ObjectNameTag, double> Weapon::addTrace(std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& from, const Vec3D& directionTo) {
    std::map<ObjectNameTag, double> damagedPlayers;

    double spreading = _spreading*ShooterConsts::FIRE_DISTANCE/100;

    //generate random vector
    Vec3D randV(spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

    // damage player
    auto rayCast = rayCastFunction(from, from + directionTo * ShooterConsts::FIRE_DISTANCE + randV);
    if(rayCast.second.str().find("Enemy") != std::string::npos) {
        damagedPlayers[rayCast.second] += _damage/(1.0 + (from - rayCast.first).abs());
    }

    // add trace line
    Vec3D to = rayCast.first == Vec3D(0) ? from + directionTo * ShooterConsts::FIRE_DISTANCE + randV: rayCast.first;
    _addTraceCallBack(from, to);

    return damagedPlayers;
}
