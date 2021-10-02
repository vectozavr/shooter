//
// Created by Иван Ильин on 01.06.2021.
//

#include <sstream>
#include <utility>
#include "Weapon.h"
#include "ResourceManager.h"
#include "utils/Log.h"
#include "animation/AColor.h"
#include "animation/AFunction.h"

using namespace std;

Weapon::Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Point4D& scale, const Point4D& t, const Point4D& r) {
    _name = weaponName;

    /*
    auto objs = Mesh::LoadObjects(objFileName, matFileName, scale);

    for(int i = 0; i < objs.size(); i++) {
        string meshName = _name + "_" + to_string(i);

        RigidBody obj(*objs[i]);

        obj.setCollider(false);

        //transforms
        obj.rotate(rotate);
        obj.translate(translate);

        _objects.insert({meshName, std::make_shared<RigidBody>(obj)});
    }
     */
    loadObj(objFileName, matFileName, scale);
    setCollider(false);
    rotate(r);
    translate(t);

    noAmmoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/no_ammo.ogg"));
}

std::map<std::string, double> Weapon::fire(std::function<std::pair<Point4D, std::string>(const Point4D&, const Point4D&)> rayCastFunction, const Point4D& position, const Point4D& direction) {
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

std::map<std::string, double> Weapon::processFire(std::function<std::pair<Point4D, std::string>(const Point4D&, const Point4D&)> rayCastFunction, const Point4D& pos, const Point4D& direction) {
    std::map<std::string, double> damagedPlayers;

    //generate random vector
    Point4D randV(10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX), 10.0*_spreading*(1.0 - 2.0*(double)rand()/RAND_MAX));

    // damage player
    auto rayCast = rayCastFunction(pos, pos + direction * 1000 + randV);
    if(rayCast.second.find("Player") != std::string::npos) {
        damagedPlayers[rayCast.second] += _damage/(1.0 + (pos - rayCast.first).abs());
    }

    // add trace line
    Point4D to = rayCast.first.w() == -1 ? pos + direction * 1000 + randV: rayCast.first;
    Point4D from = position() + triangles().back()[0];
    _addTraceCallBack(from, to);

    return damagedPlayers;
}
