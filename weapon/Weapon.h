//
// Created by Иван Ильин on 01.06.2021.
//

#ifndef SHOOTER_WEAPON_H
#define SHOOTER_WEAPON_H


#include <string>
#include "../engine/World.h"
#include "../engine/Camera.h"
#include <SFML/Audio/Sound.hpp>
#include "../engine/Mesh.h"
#include "../engine/utils/Time.h"
#include "../engine/SoundController.h"
#include "../engine/Consts.h"

struct FireInformation {
    const std::map<ObjectNameTag, double> damagedPlayers;
    const bool shot;
};

class Weapon : public RigidBody {
protected:
    int _initialPack = 100; // how much ammo do you have when you find the weapon

    int _clipCapacity = 30; // how much ammo can be stored in one clip
    int _stockAmmo = _initialPack - _clipCapacity; // how much ammo do you have in stock
    int _clipAmmo = _clipCapacity; // how much ammo do you have in current clip

    double _reloadTime = 3;
    double _fireDelay = 0.1; // time delay between fires
    double _damage = 300;

    double _spreading = 2.0;

    std::string _name = "Weapon";

    double _lastFireTime = std::numeric_limits<double>::min();
    double _lastReloadTime = std::numeric_limits<double>::min();

    std::string fireSound;
    std::string reloadSound;

    std::function<void(const Vec3D&, const Vec3D&)> _addTraceCallBack;

    std::map<ObjectNameTag, double> addTrace(std::function<IntersectionInformation(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& position, const Vec3D& direction);

    virtual std::map<ObjectNameTag, double> processFire(std::function<IntersectionInformation(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& position, const Vec3D& direction);

public:
    Weapon(const std::string& weaponName, const std::string& objFileName, const Vec3D& scale, const Vec3D& translate, const Vec3D& rotate);

    FireInformation fire(std::function<IntersectionInformation(const Vec3D&, const Vec3D&)> rayCastFunction, const Vec3D& position, const Vec3D& direction);
    void reload();

    [[nodiscard]] std::pair<double, double> balance() const{ return std::make_pair(_clipAmmo, _stockAmmo); }

    void setAddTraceCallBack(std::function<void(Vec3D, Vec3D)> add) {
        _addTraceCallBack = std::move(add);
    }

    [[nodiscard]] ObjectNameTag name() const { return ObjectNameTag(_name); }

    void addAmmo(int ammoAdd) { _stockAmmo += ammoAdd; }

    [[nodiscard]] int initialPack() const {return _initialPack; }
};


#endif //SHOOTER_3DZAVR_WEAPON_H
