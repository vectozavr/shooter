//
// Created by Иван Ильин on 05.06.2021.
//

#ifndef SHOOTER_BONUS_H
#define SHOOTER_BONUS_H

#include "engine/World.h"
#include "Player.h"

class Bonus final : public RigidBody {
private:
    std::string _name;
public:
    explicit Bonus(const std::string &bonusName, const std::string& filename, const Vec3D& scale = Vec3D{1, 1, 1});

    [[nodiscard]] std::string name() const { return _name; }
};


#endif //SHOOTER_3DZAVR_BONUS_H
