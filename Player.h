//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef SHOOTER_PLAYER_H
#define SHOOTER_PLAYER_H

#include <SFML/Audio/Sound.hpp>
#include <utility>
#include "engine/ResourceManager.h"
#include "engine/Camera.h"
#include "engine/World.h"
#include "weapon/Ak47.h"
#include "weapon/Shotgun.h"
#include "weapon/Gun.h"
#include "weapon/Gold_Ak47.h"
#include "weapon/Rifle.h"

class Player : public RigidBody{
private:
    double _healthMax = 100;
    double _health = _healthMax;

    double _abilityMax = 10;
    double _ability = _abilityMax;

    double _headAngle = 0;

    int _kills = 0;
    int _deaths = 0;

    double _g = 35;

    // sounds
    sf::Sound _killSound;
    sf::Sound _deathSound;
    sf::Sound _changeWeaponSound;
    sf::Sound _fullHealthSound;
    sf::Sound _fullAbilitySound;

    std::vector<std::shared_ptr<Weapon>> _weapons;
    size_t _selectedWeapon = 0;

    std::function<void(sf::Uint16 targetId, double)> _damagePlayerCallBack;
    std::function<void(const Vec3D&, const Vec3D&)> _addTraceCallBack;
    std::function<void(const std::string&)> _takeBonusCallBack;

    std::function<void(std::shared_ptr<Weapon>)> _addWeaponCallBack;
    std::function<void(std::shared_ptr<Weapon>)> _removeWeaponCallBack;

    std::function<std::pair<Vec3D, std::string>(const Vec3D&, const Vec3D&)> _rayCastFunction;
public:
    Player() {
        loadObj("obj/cube.obj", "", Vec3D{0.5, 1.9, 0.5});
        setAcceleration(Vec3D{0, -_g, 0});
        setCollision(true);
        setVisible(false);
        setColor({240, 168, 168});

        _changeWeaponSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/weapons/change_weapon.ogg"));

        _fullHealthSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/fullHealth.ogg"));
        _fullAbilitySound.setBuffer(*ResourceManager::loadSoundBuffer("sound/fullAbility.ogg"));

        setCollisionCallBack([this](const std::string& objName, std::shared_ptr<RigidBody> obj) {collisionWithObject(objName, obj);});
    };

    void setHealth(double h) {
        _health = h;
    }
    void setAbility(double a) {
        _ability = a;
    }

    [[nodiscard]] double health() const { return _health; }
    [[nodiscard]] double ability() const { return _ability; }

    [[nodiscard]] double maxHealth() const { return _healthMax; }
    [[nodiscard]] double maxAbility() const { return _abilityMax; }

    void setFullHealth() {
        _health = _healthMax;
        _fullHealthSound.play();
    }
    void setFullAbility() {
        _ability = _abilityMax;
        _fullAbilitySound.play();
    }

    void initWeapons();
    void addWeapon(std::shared_ptr<Weapon> weapon);
    [[nodiscard]] std::pair<double, double> balance() const{ return _weapons[_selectedWeapon]->balance();}

    void nextWeapon();
    void previousWeapon();
    void fire();
    void reload();

    void rotateWeaponsRelativePoint(const Vec3D& point, const Vec3D& v, double val);

    [[nodiscard]] int kills() const {return _kills;}
    [[nodiscard]] int deaths() const {return _deaths;}

    void addKill() { _kills++; }
    void addDeath() { _deaths++; }

    void playDeath();
    void playKill();

    void setDamagePlayerCallBack(std::function<void(sf::Uint16 targetId, double)> hit) {
        _damagePlayerCallBack = std::move(hit);
    }
    void setAddTraceCallBack(std::function<void(const Vec3D&, const Vec3D&)> add) {
        _addTraceCallBack = std::move(add);
    }
    void setTakeBonusCallBack(std::function<void(const std::string&)> take) {
        _takeBonusCallBack = std::move(take);
    }
    void setAddWeaponCallBack(std::function<void(std::shared_ptr<Weapon>)> addWeapon) {
        _addWeaponCallBack = addWeapon;
    }
    void setRemoveWeaponCallBack(std::function<void(std::shared_ptr<Weapon>)> removeWeapon) {
        _removeWeaponCallBack = std::move(removeWeapon);
    }
    void setRayCastFunction(std::function<std::pair<Vec3D, std::string>(const Vec3D&, const Vec3D&)> rayCastFunction) {
        _rayCastFunction = std::move(rayCastFunction);
    }

    // This is for situation when you want to store the position of the head but you dont have attached camera
    void setHeadAngle(double a) { _headAngle = a; }
    [[nodiscard]] double headAngle() const { return _headAngle; };

    void collisionWithObject(const std::string& objName, std::shared_ptr<RigidBody> obj);
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
