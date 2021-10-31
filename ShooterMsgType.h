//
// Created by Иван Ильин on 27.10.2021.
//

#ifndef SHOOTER_SHOOTERMSGTYPE_H
#define SHOOTER_SHOOTERMSGTYPE_H

#include <SFML/Network.hpp>

enum class ShooterMsgType {
    Damage,
    Kill,
    FireTrace,
    InitBonuses,
    AddBonus,
    RemoveBonus,
    ChangeWeapon
};

sf::Packet &operator<<(sf::Packet &packet, ShooterMsgType type);

sf::Packet &operator>>(sf::Packet &packet, ShooterMsgType &type);

#endif //SHOOTER_SHOOTERMSGTYPE_H
