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

#include "../engine/Consts.h"

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

    std::string _name = "Weapon_name";

    double _lastFireTime = std::numeric_limits<double>::min();
    double _lastReloadTime = std::numeric_limits<double>::min();

    sf::Sound fireSound;
    sf::Sound reloadSound;
    sf::Sound noAmmoSound;

    int fireTraces = 0;

    std::function<void(const Point4D&, const Point4D&)> _addTraceCallBack;

    virtual std::map<std::string, double> processFire(std::function<std::pair<Point4D, std::string>(const Point4D&, const Point4D&)> rayCastFunction, const Point4D& position, const Point4D& direction);

public:
    Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Point4D& scale, const Point4D& translate, const Point4D& rotate);

    std::map<std::string, double> fire(std::function<std::pair<Point4D, std::string>(const Point4D&, const Point4D&)> rayCastFunction, const Point4D& position, const Point4D& direction);
    void reload();

    [[nodiscard]] std::pair<double, double> balance() const{ return std::make_pair(_clipAmmo, _stockAmmo); }

    void setAddTraceCallBack(std::function<void(Point4D, Point4D)> add) {
        _addTraceCallBack = std::move(add);
    }

    [[nodiscard]] std::string name() const { return _name; }

    void addAmmo(int ammoAdd) { _stockAmmo += ammoAdd; }

    [[nodiscard]] int initialPack() const {return _initialPack; }
};


#endif //SHOOTER_3DZAVR_WEAPON_H
