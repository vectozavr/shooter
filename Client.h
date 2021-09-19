//
// Created by Иван Ильин on 25.05.2021.
//

#ifndef SHOOTER_CLIENT_H
#define SHOOTER_CLIENT_H

#include <utility>

#include "network/ClientUDP.h"
#include "Player.h"

class Client : public ClientUDP {
private:
    std::shared_ptr<Player> _player;
    std::shared_ptr<World> _world;

    std::map<sf::Uint16, std::shared_ptr<Player>> _players{};

    int fireTraces = 0;

    void spawnPlayer(sf::Uint16 id);
public:
    Client(std::shared_ptr<Player>  player, std::shared_ptr<World>  world) : _player(player), _world(world) {};

    void updatePacket() override;

    void processInit(sf::Packet& packet) override;
    void processUpdate(sf::Packet& packet) override;
    void processNewClient(sf::Packet& packet) override;
    void processDisconnect(sf::Uint16 targetId) override;

    void processCustomPacket(MsgType type, sf::Packet& packet) override;

    void processDisconnected() override;

    void damagePlayer(sf::Uint16 targetId, double damage);

    void takeBonus(const std::string& bonusName);

    void addTrace(const Point4D& from, const Point4D& to);

    void deleteTrace(std::shared_ptr<World> world, const std::string& traceName);
};


#endif //MINECRAFT_3DZAVR_CLIENT_H
