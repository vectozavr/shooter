//
// Created by Иван Ильин on 05.06.2021.
//

#ifndef SHOOTER_BONUS_H
#define SHOOTER_BONUS_H

#include "World.h"
#include "Player.h"

class Bonus : public RigidBody {
protected:
    std::string _name;
public:
    explicit Bonus(const std::string &bonusName, const std::string& filename, const std::string &materials = "", const Point4D& scale = Point4D{1, 1, 1});

    [[nodiscard]] std::string name() const { return _name; }
};


#endif //SHOOTER_3DZAVR_BONUS_H
