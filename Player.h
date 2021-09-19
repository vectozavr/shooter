//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef SHOOTER_PLAYER_H
#define SHOOTER_PLAYER_H

#include <SFML/Audio/Sound.hpp>
#include <utility>
#include <ResourceManager.h>
#include "Camera.h"
#include "World.h"
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

    double _jumpHeight = 3;
    double _walkSpeed = 10;
    double _headAngle = 0;

    unsigned _kills = 0;
    unsigned _deaths = 0;

    double _g = 35;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Screen> _screen;

    std::shared_ptr<World> _world;

    // sounds
    sf::Sound _killSound;
    sf::Sound _deathSound;
    sf::Sound _changeWeaponSound;
    sf::Sound _fullHealthSound;
    sf::Sound _fullAbilitySound;

    std::string _name = "im";

    std::vector<std::shared_ptr<Weapon>> _weapons;
    uint8_t _selectedWeapon = 0;

    std::function<void(sf::Uint16 targetId, double)> _damagePlayerCallBack;
    std::function<void(const Point4D&, const Point4D&)> _addTraceCallBack;
    std::function<void(const std::string&)> _takeBonusCallBack;

public:
    Player() {
        loadObj("../obj/cube.obj", "", Point4D{0.5, 1.9, 0.5});
        setAcceleration(Point4D{0, -_g, 0});
        setCollision(true);
        setVisible(false);
        setColor({240, 168, 168});

        _changeWeaponSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/change_weapon.ogg"));

        _fullHealthSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/fullHealth.ogg"));
        _fullAbilitySound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/fullAbility.ogg"));

        setCollisionCallBack([this](const std::string& objName, std::shared_ptr<RigidBody> obj) {collisionWithObject(objName, obj);});
    };

    void update();

    void attachCamera(std::shared_ptr<Camera> camera, std::shared_ptr<Screen> screen) {
        _camera = camera;
        _screen = screen;

        _camera->translateToPoint(position() + Point4D{0, 1.8, 0});
        this->attach(_camera);
    }

    void attachWorld(std::shared_ptr<World> world, const Point4D& pos = Point4D{0, 30, 0}) {
        _world = world;

        translate(pos);

        initWeapons();
    }

    void setHealth(double h) {
        _health = h;
    }
    void setAbility(double a) {
        _ability = a;
    }

    void nextWeapon();
    void previousWeapon();
    void fire();
    void reload();

    [[nodiscard]] double ability() const { return _ability; }

    void setFullHealth() {
        _health = _healthMax;
        _fullHealthSound.play();
    }
    void setFullAbility() {
        _ability = _abilityMax;
        _fullAbilitySound.play();
    }


    void setDamagePlayerCallBack(std::function<void(sf::Uint16 targetId, double)> hit) {
        _damagePlayerCallBack = std::move(hit);
    }

    void setAddTraceCallBack(std::function<void(const Point4D&, const Point4D&)> add) {
        _addTraceCallBack = std::move(add);
    }

    void setTakeBonusCallBack(std::function<void(const std::string&)> take) {
        _takeBonusCallBack = std::move(take);
    }

    [[nodiscard]] double health() const { return _health; }
    [[nodiscard]] std::string name() const { return "Player_" + _name; }


    std::shared_ptr<Camera> camera() { return _camera; }

    // This is for situation when you want to store the position of the head but you dont have attached camera
    void setHeadAngle(double a) { _headAngle = a; }
    [[nodiscard]] double headAngle() const { return _headAngle; };

    void rotateWeaponsRelativePoint(const Point4D& point4D, const Point4D& v, double val);

    void drawStats();

    void addKill() { _kills++; }
    void addDeath() { _deaths++; }

    void playDeath();
    void playKill();

    void collisionWithObject(const std::string& objName, std::shared_ptr<RigidBody> obj);

    void addWeapon(std::shared_ptr<Weapon> weapon);

    void initWeapons();
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
