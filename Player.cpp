//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "utils/Log.h"

void Player::update() {

}

void Player::rotateWeaponsRelativePoint(const Point4D& point4D, const Point4D& v, double val) {
    for(auto& weapon : _weapons)
        weapon->rotateRelativePoint(point4D, v, val);
}

void Player::drawStats() {
    if(_screen == nullptr)
        return;

    // health bar
    double xPos = 10;
    double yPos = _screen->height() - 10 - 10;

    double width = _screen->width()/2 - 20;
    double height = 10;

    _screen->drawTetragon(Point4D{xPos, yPos},
                          Point4D{xPos + width, yPos},
                          Point4D{xPos + width, yPos + height},
                          Point4D{xPos, yPos + height},
                          { static_cast<sf::Uint8>((_healthMax - _health)/_healthMax * 255), static_cast<sf::Uint8>(_health * 255 / _healthMax), 0, 100 });

    _screen->drawTetragon(Point4D{xPos, yPos - 15},
                          Point4D{xPos + width * _ability / _abilityMax, yPos - 15},
                          Point4D{xPos + width * _ability / _abilityMax, yPos - 15 + height},
                          Point4D{xPos, yPos - 15 + height},
                          { 255, 168, 168, 100 });

    auto balance = _weapons[_selectedWeapon]->balance();

    _screen->drawText(std::to_string((int)balance.first),Point4D{150, static_cast<double>(_screen->height() - 50 - 100)},100, sf::Color(0, 0, 0, 100));
    _screen->drawText(std::to_string((int)balance.second),Point4D{50, static_cast<double>(_screen->height() - 50 - 50)},50, sf::Color(0, 0, 0, 70));

    _screen->drawText("KILLS: " + std::to_string((int)_kills) + " | " + "DEATHS: " + std::to_string((int)_deaths),
                      Point4D{10, 10},25, sf::Color(0, 0, 0, 100));
}

void Player::playDeath() {
    _deathSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/classic_hurt.ogg"));
    _deathSound.play();
}

void Player::playKill() {
    _killSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/kill.ogg"));
    _killSound.play();
}

void Player::collisionWithObject(const std::string &objName, std::shared_ptr<RigidBody> obj) {
    if(objName.find("Bonus_gun") != std::string::npos)
        addWeapon(std::make_shared<Gun>());

    if(objName.find("Bonus_shotgun") != std::string::npos)
        addWeapon(std::make_shared<Shotgun>());

    if(objName.find("Bonus_ak47") != std::string::npos)
        addWeapon(std::make_shared<Ak47>());

    if(objName.find("Bonus_gold_ak47") != std::string::npos)
        addWeapon(std::make_shared<Gold_Ak47>());

    if(objName.find("Bonus_rifle") != std::string::npos)
        addWeapon(std::make_shared<Rifle>());

    if(objName.find("Bonus_hill") != std::string::npos)
        setFullHealth();

    if(objName.find("Bonus_ability") != std::string::npos)
        setFullAbility();

    if(objName.find("Bonus") != std::string::npos) {
        _world->removeBody(objName);
        _takeBonusCallBack(objName);
    }
}

void Player::addWeapon(std::shared_ptr<Weapon> weapon) {
    _changeWeaponSound.play();

    if(!_weapons.empty()) {
        for(auto& w : _weapons) {
            if (w->name() == weapon->name()) {
                w->addAmmo(w->initialPack());
                return;
            }
        }
    }

    _weapons.push_back(weapon);
    _weapons.back()->attachToPlayer(*this);

    _weapons.back()->translate(position());
    _weapons.back()->rotateRelativePoint(position() + Point4D{0, 1.8, 0}, Point4D{0, 1, 0}, _angle.y());
    _weapons.back()->rotateRelativePoint(position() + Point4D{0, 1.8, 0}, _camera->left(), _camera->angleLeftUpLookAt().x());

    _weapons.back()->setAddTraceCallBack(_addTraceCallBack);
    _changeWeaponSound.play();
}

void Player::initWeapons() {

    if(!_weapons.empty()) {
        _weapons[_selectedWeapon]->removeFromWorld(_world);
        _weapons.clear();
    }

    _selectedWeapon = 0;
    addWeapon(std::make_shared<Gun>());

    _weapons[_selectedWeapon]->addToWorld(_world);
}

void Player::nextWeapon() {
    if(_weapons.size() > 1) {
        // change '_selectedWeapon'
        _weapons[_selectedWeapon]->removeFromWorld(_world);
        _selectedWeapon = (_selectedWeapon + 1) % _weapons.size();
        _weapons[_selectedWeapon]->addToWorld(_world);
        Log::log("selected _selectedWeapon " + std::to_string(_selectedWeapon));
        _changeWeaponSound.play();
    }
}

void Player::previousWeapon() {
    if(_weapons.size() > 1) {
        // change '_selectedWeapon'
        _weapons[_selectedWeapon]->removeFromWorld(_world);
        if (_selectedWeapon > 0)
            _selectedWeapon = (_selectedWeapon - 1) % _weapons.size();
        else
            _selectedWeapon = _weapons.size() - 1;
        _weapons[_selectedWeapon]->addToWorld(_world);
        Log::log("selected _selectedWeapon " + std::to_string(_selectedWeapon));
        _changeWeaponSound.play();
    }
}

void Player::fire() {
    auto damagedPlayers = _weapons[_selectedWeapon]->fire(_world, _camera);
    for(auto& damagedPlayer : damagedPlayers) {
        sf::Uint16 targetId = std::stoi(damagedPlayer.first.substr(7));
        _damagePlayerCallBack(targetId, damagedPlayer.second);
    }
}

void Player::reload() {
    _weapons[_selectedWeapon]->reload();
}
