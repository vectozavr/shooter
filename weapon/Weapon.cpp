//
// Created by Иван Ильин on 01.06.2021.
//

#include "Weapon.h"
#include "../engine/utils/ResourceManager.h"
#include "../engine/utils/Log.h"
#include "../ShooterConsts.h"

using namespace std;

Weapon::Weapon(int initialPack, int clipCapacity, double reloadTime, double fireDelay, double damage, double spreading,
               std::string fireSound, std::string reloadSound, ObjectNameTag weaponName, const std::string &objFileName,
               const Vec3D &s, const Vec3D &t, const Vec3D &r) : RigidBody(std::move(weaponName), objFileName),
                                                                 _initialPack(initialPack), _clipCapacity(clipCapacity),
                                                                 _reloadTime(reloadTime), _fireDelay(fireDelay),
                                                                 _damage(damage), _spreading(spreading),
                                                                 _fireSound(std::move(fireSound)),
                                                                 _reloadSound(std::move(reloadSound)) {
    _stockAmmo = _initialPack - _clipCapacity;
    _clipAmmo = _clipCapacity;

    loadObj(objFileName, s);
    setCollider(false);
    rotate(r);
    translate(t);
}

FireInformation Weapon::fire(std::function<IntersectionInformation(const Vec3D &, const Vec3D &)> rayCastFunction,
                             const Vec3D &position, const Vec3D &direction) {
    if (_clipAmmo == 0) {
        reload();
        if (_clipAmmo == 0 && SoundController::getStatus(SoundTag("noAmmo")) != sf::Sound::Status::Playing) {
            SoundController::loadAndPlay(SoundTag("noAmmo"), ShooterConsts::NO_AMMO_SOUND);
        }
    }

    if (_clipAmmo <= 0 || std::abs(Time::time() - _lastFireTime) < _fireDelay ||
        std::abs(Time::time() - _lastReloadTime) < _reloadTime) {
        return FireInformation{std::map<ObjectNameTag, double>(), false};
    }

    _lastFireTime = Time::time();
    _clipAmmo--;

    SoundController::loadAndPlay(SoundTag("fireSound_" + name().str()), _fireSound);
    Log::log("Weapon::fire (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");

    if (_fireCallBack != nullptr) {
        _fireCallBack();
    }

    return FireInformation{processFire(std::move(rayCastFunction), position, direction), true};
}

void Weapon::reload() {
    if (_stockAmmo == 0 || std::abs(Time::time() - _lastReloadTime) < _reloadTime || _clipAmmo == _clipCapacity) {
        return;
    }
    if (_clipCapacity - _clipAmmo <= _stockAmmo) {
        _stockAmmo -= _clipCapacity - _clipAmmo;
        _clipAmmo = _clipCapacity;
    } else {
        _clipAmmo += _stockAmmo;
        _stockAmmo = 0;
    }

    SoundController::loadAndPlay(SoundTag("reloadSound_" + name().str()), _reloadSound);
    Log::log("Weapon::reload (" + std::to_string(_stockAmmo) + " : " + std::to_string(_clipAmmo) + ")");
    _lastReloadTime = Time::time();

    if (_reloadCallBack != nullptr) {
        _reloadCallBack();
    }
}

std::map<ObjectNameTag, double>
Weapon::processFire(std::function<IntersectionInformation(const Vec3D &, const Vec3D &)> rayCastFunction,
                    const Vec3D &position, const Vec3D &direction) const {
    // Standard weapon usually fire one bullet at a time
    // But some types of weapon can fire several bullet at the same time (for ex. shotgun)
    // That's why processFire() is s virtual function
    return fireABullet(std::move(rayCastFunction), position, direction);
}

std::map<ObjectNameTag, double>
Weapon::fireABullet(std::function<IntersectionInformation(const Vec3D &, const Vec3D &)> rayCastFunction,
                    const Vec3D &cameraPosition, const Vec3D &direction) const {
    std::map<ObjectNameTag, double> damagedPlayers;

    double spreading = _spreading * ShooterConsts::FIRE_DISTANCE / 100.0;

    //generate random vector
    Vec3D randV(spreading * (1.0 - 2.0 * static_cast<double>(rand()) / RAND_MAX),
                spreading * (1.0 - 2.0 * static_cast<double>(rand()) / RAND_MAX),
                spreading * (1.0 - 2.0 * static_cast<double>(rand()) / RAND_MAX));

    // damage player
    auto rayCast = rayCastFunction(cameraPosition, cameraPosition + direction * ShooterConsts::FIRE_DISTANCE + randV);
    if (rayCast.objectName.contains(ObjectNameTag("Enemy"))) {

        damagedPlayers[rayCast.objectName] += _damage / (1.0 + rayCast.distanceToObject);

        // If you hit the head the damage will be doubled
        if (rayCast.objectName.contains(ObjectNameTag("head"))) {
            damagedPlayers[rayCast.objectName] += _damage / (1.0 + rayCast.distanceToObject);
        }
        // If you hit the foot the damage will be divided by 2
        if (rayCast.objectName.contains(ObjectNameTag("foot"))) {
            damagedPlayers[rayCast.objectName] -= 0.5 * _damage / (1.0 + rayCast.distanceToObject);
        }
    }

    // add trace line
    Vec3D lineFrom = position() + model() * Vec3D(triangles().back()[0]);
    Vec3D lineTo = rayCast.intersected ? rayCast.pointOfIntersection : position() +
                                                                       direction * ShooterConsts::FIRE_DISTANCE +
                                                                       randV;
    _addTraceCallBack(lineFrom, lineTo);

    return damagedPlayers;
}