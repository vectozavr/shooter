//
// Created by Иван Ильин on 14.03.2021.
//

#ifndef SHOOTER_PLAYER_H
#define SHOOTER_PLAYER_H

#include <SFML/Audio/Sound.hpp>
#include <utility>
#include <ResourceManager.h>
#include "Mesh.h"
#include "Camera.h"
#include "World.h"
#include "Ak47.h"
#include "Shotgun.h"
#include "Gun.h"
#include "Gold_Ak47.h"
#include "Rifle.h"

class Player : public Mesh{
private:
    double _healthMax = 100;
    double _health = _healthMax;

    double _abilityMax = 10;
    double _ability = _abilityMax;

    double jumpHeight = 3;
    double walkSpeed = 10;
    double _headAngle = 0;

    unsigned _kills = 0;
    unsigned _deaths = 0;

    double g = 35;

    double slowMoCoefficient = 5;
    bool isInSlowMo = false;

    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Screen> _screen;

    std::shared_ptr<World> _world;

    bool inRunning = false;

    // sounds
    sf::Sound killSound;
    sf::Sound deathSound;
    sf::Sound walkSound;
    sf::Sound changeWeaponSound;
    sf::Sound slowMoSound;
    sf::Sound unSlowMoSound;
    sf::Sound fullHealthSound;
    sf::Sound fullAbilitySound;


    Point4D oldVelocity;

    std::string _name = "im";

    std::vector<std::shared_ptr<Weapon>> _weapons;
    uint8_t _selectedWeapon = 0;

    std::function<void(sf::Uint16 targetId, double)> damagePlayerCallBack;
    std::function<void(const Point4D&, const Point4D&)> addTraceCallBack;
    std::function<void(const std::string&)> takeBonusCallBack;


public:
    Player() {
        loadObj("../obj/cube.obj", "", Point4D{0.5, 1.9, 0.5});
        setAcceleration(Point4D{0, -g, 0});
        setCollision(true);
        setVisible(false);
        setColor({240, 168, 168});
    };

    void update();

    void attachCamera(std::shared_ptr<Camera>& camera, std::shared_ptr<Screen> screen) {
        _camera = camera;
        _screen = std::move(screen);

        camera->translateToPoint(position() + Point4D{0, 1.8, 0});
        this->attach(camera);
    }

    void attachWorld(const std::shared_ptr<World>& world, const Point4D& pos = Point4D{0, 30, 0}) {
        _world = world;

        translate(pos);

        initWeapons();

        changeWeaponSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/weapons/change_weapon.ogg"));

        slowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/slow_mo.ogg"));
        unSlowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/unslow_mo.ogg"));

        fullHealthSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/fullHealth.ogg"));
        fullAbilitySound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/fullAbility.ogg"));

        setCollisionCallBack([this](const std::string& objName, const std::shared_ptr<Mesh>& obj) {collisionWithObject(objName, obj);});
    }

    void setHealth(double h) {
        _health = h;
    }
    void setAbility(double a) {
        _ability = a;
    }

    void setFullHealth() {
        _health = _healthMax;
        fullHealthSound.play();
    }
    void setFullAbility() {
        _ability = _abilityMax;
        fullAbilitySound.play();
    }


    void setDamagePlayerCallBack(std::function<void(sf::Uint16 targetId, double)> hit) {
        damagePlayerCallBack = std::move(hit);
    }

    void setAddTraceCallBack(std::function<void(const Point4D&, const Point4D&)> add) {
        addTraceCallBack = std::move(add);
    }

    void setTakeBonusCallBack(std::function<void(const std::string&)> take) {
        takeBonusCallBack = std::move(take);
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

    void collisionWithObject(const std::string& objName, const std::shared_ptr<Mesh>& obj);

    void addWeapon(const std::shared_ptr<Weapon>& weapon);

    void initWeapons();
};


#endif //MINECRAFT_3DZAVR_PLAYER_H
