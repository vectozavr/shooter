//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "engine/Screen.h"
#include "engine/utils/Log.h"

#include "engine/animation/Timeline.h"
#include "engine/animation/ARotateLeft.h"

Player::Player(ObjectNameTag name) : RigidBody(name) {
    loadObj(ShooterConsts::CUBE_OBJ, Vec3D{0.5, 1.9, 0.5});
    setAcceleration(Vec3D{0, -ShooterConsts::GRAVITY, 0});
    setCollision(true);
    setVisible(false);

    Vec3D randColor = Vec3D::Random();
    setColor({static_cast<sf::Uint8>(randColor.x() * 255), static_cast<sf::Uint8>(randColor.y() * 255),
              static_cast<sf::Uint8>(randColor.z() * 255)});

    setCollisionCallBack(
            [this](const ObjectNameTag &tag, std::shared_ptr<RigidBody> obj) { collisionWithObject(tag, obj); });
}

void Player::rotateWeaponsRelativePoint(const Vec3D &point4D, const Vec3D &v, double val) {
    for (auto &weapon : _weapons) {
        weapon->rotateRelativePoint(point4D, v, val);
    }
}

void Player::collisionWithObject(const ObjectNameTag &tag, std::shared_ptr<RigidBody> obj) {
    if (tag.str().find("Bonus_gun") != std::string::npos) {
        addWeapon(std::make_shared<Gun>());
    }

    if (tag.str().find("Bonus_shotgun") != std::string::npos) {
        addWeapon(std::make_shared<Shotgun>());
    }

    if (tag.str().find("Bonus_ak47") != std::string::npos) {
        addWeapon(std::make_shared<Ak47>());
    }

    if (tag.str().find("Bonus_gold_ak47") != std::string::npos) {
        addWeapon(std::make_shared<Gold_Ak47>());
    }

    if (tag.str().find("Bonus_rifle") != std::string::npos) {
        addWeapon(std::make_shared<Rifle>());
    }

    if (tag.str().find("Bonus_hill") != std::string::npos) {
        setFullHealth();
    }

    if (tag.str().find("Bonus_ability") != std::string::npos) {
        setFullAbility();
    }

    if (tag.str().find("Bonus") != std::string::npos) {
        if (_takeBonusCallBack != nullptr) {
            _takeBonusCallBack(tag.str());
        }
    }
}

void Player::addWeapon(std::shared_ptr<Weapon> weapon) {
    SoundController::playSound(SoundTag("changeWeapon"), ShooterConsts::CHANGE_WEAPON_SOUND);

    for (auto &w : _weapons) {
        if (w->name() == weapon->name()) {
            w->addAmmo(w->initialPack());
            return;
        }
    }

    _weapons.push_back(weapon);
    attach(weapon);

    _weapons.back()->translate(position());
    _weapons.back()->rotateRelativePoint(position() + Vec3D{0, 1.8, 0}, Vec3D{0, 1, 0}, angle().y());
    _weapons.back()->rotateRelativePoint(position() + Vec3D{0, 1.8, 0}, left(), headAngle());

    // add animation of reloading
    _weapons.back()->setReloadCallBack([this]() {
        Timeline::animate(AnimationListTag("reload_weapon"),
                          std::make_shared<ARotateLeft>(_weapons[_selectedWeapon],
                                                        4 * Consts::PI,
                                                        _weapons[_selectedWeapon]->reloadTime()/2,
                                                        Animation::LoopOut::None,
                                                        Animation::InterpolationType::Cos));
    });

    // add call back function to create fire traces
    if (_addTraceCallBack != nullptr) {
        _weapons.back()->setAddTraceCallBack(_addTraceCallBack);
    }
}

void Player::reInitWeapons() {

    if (!_weapons.empty()) {
        for (auto weapon : _weapons) {
            unattach(ObjectNameTag(weapon->name()));
        }

        if (_removeWeaponCallBack != nullptr) {
            _removeWeaponCallBack(_weapons[_selectedWeapon]);
        }
        _weapons.clear();
    }

    _selectedWeapon = 0;
    addWeapon(std::make_shared<Gun>());
    if (_addWeaponCallBack != nullptr) {
        _addWeaponCallBack(_weapons[_selectedWeapon]);
    }
}

void Player::selectNextWeapon() {
    if (_weapons.size() > 1) {
        // change '_selectedWeapon'
        if (_removeWeaponCallBack != nullptr) {
            _removeWeaponCallBack(_weapons[_selectedWeapon]);
        }

        _selectedWeapon = (_selectedWeapon + 1) % _weapons.size();

        if (_addWeaponCallBack != nullptr) {
            _addWeaponCallBack(_weapons[_selectedWeapon]);
        }
        Log::log("selectedWeapon " + std::to_string(_selectedWeapon));
        SoundController::playSound(SoundTag("changeWeapon"), ShooterConsts::CHANGE_WEAPON_SOUND);
    }
}

void Player::selectPreviousWeapon() {
    if (_weapons.size() > 1) {
        // change '_selectedWeapon'
        if (_removeWeaponCallBack != nullptr) {
            _removeWeaponCallBack(_weapons[_selectedWeapon]);
        }
        if (_selectedWeapon > 0) {
            _selectedWeapon = (_selectedWeapon - 1) % _weapons.size();
        } else {
            _selectedWeapon = _weapons.size() - 1;
        }
        if (_addWeaponCallBack != nullptr) {
            _addWeaponCallBack(_weapons[_selectedWeapon]);
        }
        Log::log("selectedWeapon " + std::to_string(_selectedWeapon));
        SoundController::playSound(SoundTag("changeWeapon"), ShooterConsts::CHANGE_WEAPON_SOUND);
    }
}

bool Player::fire() {
    auto camera = attached(ObjectNameTag("Camera"));
    if (camera != nullptr) {
        auto fireInfo = _weapons[_selectedWeapon]->fire(_rayCastFunction, camera->position(), camera->lookAt());
        for (auto&[damagedPlayerName, damage] : fireInfo.damagedPlayers) {
            sf::Uint16 targetId = std::stoi(damagedPlayerName.str().substr(6));
            if (_damagePlayerCallBack != nullptr) {
                _damagePlayerCallBack(targetId, damage);
            }
        }
        return fireInfo.shot;
    }
    return false;
}

void Player::reload() {
    _weapons[_selectedWeapon]->reload();
}

void Player::setFullHealth() {
    _health = ShooterConsts::HEALTH_MAX;
    SoundController::playSound(SoundTag("addHealth"), ShooterConsts::RESTORE_HEALTH_SOUND);
}

void Player::setFullAbility() {
    _ability = ShooterConsts::ABILITY_MAX;
    SoundController::playSound(SoundTag("addAbility"), ShooterConsts::RESTORE_ABILITY_SOUND);
}
