//
// Created by Иван Ильин on 02.06.2021.
//

#ifndef SHOOTER_SHOTGUN_H
#define SHOOTER_SHOTGUN_H

#include "Weapon.h"
#include "../engine/ResourceManager.h"
#include "../ShooterConsts.h"

class Shotgun final : public Weapon {
public:
    explicit Shotgun(): Weapon(15, 1, 1.0, 1.0, 400, 5.0, ShooterConsts::SHOTGUN_FIRE_SOUND,
                               ShooterConsts::SHOTGUN_RELOAD_SOUND, ObjectNameTag("shotgun"), ShooterConsts::SHOTGUN_OBJ,
                               Vec3D{3, 3, 3}, Vec3D{-1.95, 0.8, 1.5}, Vec3D{0, Consts::PI, 0}) {}

    std::map<ObjectNameTag, double>
    processFire(std::function<IntersectionInformation(const Vec3D &, const Vec3D &)> rayCastFunction,
                const Vec3D &position, const Vec3D &direction) override {

        std::map<ObjectNameTag, double> damagedPlayers;

        for (int i = 0; i < 15; i++) {
            std::map<ObjectNameTag, double> damaged = addTrace(rayCastFunction, position, direction);
            for (auto &player : damaged) {
                damagedPlayers[player.first] += player.second;
            }
        }

        return damagedPlayers;
    }
};


#endif //SHOOTER_3DZAVR_SHOTGUN_H
