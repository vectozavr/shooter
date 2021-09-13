//
// Created by Neirokan on 30.04.2020
//

#ifndef ENGINE_MSGTYPE_H
#define ENGINE_MSGTYPE_H

#include <SFML/Network.hpp>

enum class MsgType
{
    // internal messages
    Empty,        // Empty message (there are no message)
    Error,        // Error message (something went wrong)
    Confirm,      // confirm receive

    // external messages
    Connect,      // connection         (client ---> server)
    Disconnect,   // disconnect         (client <==> server)
    Init,         // initialization     (client <--- server)
    Update,       // update             (client <--- server)
    ClientUpdate, // update             (client ---> server)
    NewClient,    // add new client     (client <--- server)

    // custom
    Damage,
    Kill,
    FireTrace,
    InitBonuses,
    AddBonus,
    RemoveBonus,
};

sf::Packet& operator<<(sf::Packet& packet, MsgType type);
sf::Packet& operator>>(sf::Packet& packet, MsgType& type);


#endif //INC_3DZAVR_MSGTYPE_H
