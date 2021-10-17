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
#include "ShooterConsts.h"

class Player final : public RigidBody{
private:
    double _health = ShooterConsts::HEALTH_MAX;
    double _ability = ShooterConsts::ABILITY_MAX;

    double _headAngle = 0;

    int _kills = 0;
    int _deaths = 0;

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

    std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> _rayCastFunction;
public:
    Player();

    void setHealth(double h) {
        _health = h;
    }
    void setAbility(double a) {
        _ability = a;
    }

    [[nodiscard]] double health() const { return _health; }
    [[nodiscard]] double ability() const { return _ability; }


    void setFullHealth();
    void setFullAbility();

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
        _addWeaponCallBack = std::move(addWeapon);
    }
    void setRemoveWeaponCallBack(std::function<void(std::shared_ptr<Weapon>)> removeWeapon) {
        _removeWeaponCallBack = std::move(removeWeapon);
    }
    void setRayCastFunction(std::function<std::pair<Vec3D, ObjectNameTag>(const Vec3D&, const Vec3D&)> rayCastFunction) {
        _rayCastFunction = std::move(rayCastFunction);
    }

    // This is for situation when you want to store the position of the head but you dont have attached camera
    void setHeadAngle(double a) { _headAngle = a; }
    [[nodiscard]] double headAngle() const { return _headAngle; };

    void collisionWithObject(const ObjectNameTag& tag, std::shared_ptr<RigidBody> obj);
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
