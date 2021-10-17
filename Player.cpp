//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "engine/Screen.h"
#include "engine/ResourceManager.h"
#include "engine/utils/Log.h"

Player::Player() {
    loadObj(ShooterConsts::CUBE_OBJ, "", Vec3D{0.5, 1.9, 0.5});
    setAcceleration(Vec3D{0, -ShooterConsts::GRAVITY, 0});
    setCollision(true);
    setVisible(false);
    setColor({240, 168, 168});

    _changeWeaponSound.setBuffer(*ResourceManager::loadSoundBuffer(ShooterConsts::CHANGE_WEAPON_SOUND));

    _fullHealthSound.setBuffer(*ResourceManager::loadSoundBuffer(ShooterConsts::RESTORE_HEALTH_SOUND));
    _fullAbilitySound.setBuffer(*ResourceManager::loadSoundBuffer(ShooterConsts::RESTORE_ABILITY_SOUND));

    setCollisionCallBack([this](const ObjectNameTag& tag, std::shared_ptr<RigidBody> obj) {collisionWithObject(tag, obj);});
}

void Player::rotateWeaponsRelativePoint(const Vec3D& point4D, const Vec3D& v, double val) {
    for(auto& weapon : _weapons)
        weapon->rotateRelativePoint(point4D, v, val);
}

void Player::playDeath() {
    _deathSound.setBuffer(*ResourceManager::loadSoundBuffer(ShooterConsts::DEATH_SOUND));
    _deathSound.play();
}

void Player::playKill() {
    _killSound.setBuffer(*ResourceManager::loadSoundBuffer(ShooterConsts::KILL_SOUND));
    _killSound.play();
}

void Player::collisionWithObject(const ObjectNameTag& tag, std::shared_ptr<RigidBody> obj) {
    if(tag.str().find("Bonus_gun") != std::string::npos)
        addWeapon(std::make_shared<Gun>());

    if(tag.str().find("Bonus_shotgun") != std::string::npos)
        addWeapon(std::make_shared<Shotgun>());

    if(tag.str().find("Bonus_ak47") != std::string::npos)
        addWeapon(std::make_shared<Ak47>());

    if(tag.str().find("Bonus_gold_ak47") != std::string::npos)
        addWeapon(std::make_shared<Gold_Ak47>());

    if(tag.str().find("Bonus_rifle") != std::string::npos)
        addWeapon(std::make_shared<Rifle>());

    if(tag.str().find("Bonus_hill") != std::string::npos)
        setFullHealth();

    if(tag.str().find("Bonus_ability") != std::string::npos)
        setFullAbility();

    if(tag.str().find("Bonus") != std::string::npos) {
        _takeBonusCallBack(tag.str());
    }
}

void Player::addWeapon(std::shared_ptr<Weapon> weapon) {
    _changeWeaponSound.play();

    for(auto& w : _weapons) {
        if (w->name() == weapon->name()) {
            w->addAmmo(w->initialPack());
            return;
        }
    }

    _weapons.push_back(weapon);
    attach(weapon, ObjectNameTag(weapon->name()));

    _weapons.back()->translate(position());
    _weapons.back()->rotateRelativePoint(position() + Vec3D{0, 1.8, 0}, Vec3D{0, 1, 0}, _angle->y());
    _weapons.back()->rotateRelativePoint(position() + Vec3D{0, 1.8, 0}, left(), headAngle());

    _weapons.back()->setAddTraceCallBack(_addTraceCallBack);
}

void Player::initWeapons() {

    if(!_weapons.empty()) {
        for(auto weapon : _weapons)
            unattach(ObjectNameTag(weapon->name()));

        _removeWeaponCallBack(_weapons[_selectedWeapon]);
        _weapons.clear();
    }

    _selectedWeapon = 0;
    addWeapon(std::make_shared<Gun>());
    _addWeaponCallBack(_weapons[_selectedWeapon]);
}

void Player::nextWeapon() {
    if(_weapons.size() > 1) {
        // change '_selectedWeapon'
        _removeWeaponCallBack(_weapons[_selectedWeapon]);
        _selectedWeapon = (_selectedWeapon + 1) % _weapons.size();
        _addWeaponCallBack(_weapons[_selectedWeapon]);
        Log::log("selectedWeapon " + std::to_string(_selectedWeapon));
        _changeWeaponSound.play();
    }
}

void Player::previousWeapon() {
    if(_weapons.size() > 1) {
        // change '_selectedWeapon'
        _removeWeaponCallBack(_weapons[_selectedWeapon]);
        if (_selectedWeapon > 0)
            _selectedWeapon = (_selectedWeapon - 1) % _weapons.size();
        else
            _selectedWeapon = _weapons.size() - 1;
        _addWeaponCallBack(_weapons[_selectedWeapon]);
        Log::log("selectedWeapon " + std::to_string(_selectedWeapon));
        _changeWeaponSound.play();
    }
}

void Player::fire() {
    if(attached(ObjectNameTag("camera")) != nullptr) {
        auto damagedPlayers = _weapons[_selectedWeapon]->fire(_rayCastFunction);
        for(auto& [damagedPlayerName, damage] : damagedPlayers) {
            sf::Uint16 targetId = std::stoi(damagedPlayerName.str().substr(6));
            _damagePlayerCallBack(targetId, damage);
        }
    }
}

void Player::reload() {
    _weapons[_selectedWeapon]->reload();
}

void Player::setFullHealth() {
    _health = ShooterConsts::HEALTH_MAX;
    _fullHealthSound.play();
}

void Player::setFullAbility() {
    _ability = ShooterConsts::ABILITY_MAX;
    _fullAbilitySound.play();
}
