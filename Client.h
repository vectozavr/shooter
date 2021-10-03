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
    std::map<sf::Uint16, std::shared_ptr<Player>> _players{};
    std::shared_ptr<Player> _player;

    void spawnPlayer(sf::Uint16 id);

    std::function<void(sf::Uint16)> _spawnPlayerCallBack;
    std::function<void(sf::Uint16)> _removePlayerCallBack;
    std::function<void(const Point4D&, const Point4D&)> _addFireTraceCallBack;
    std::function<void(const std::string&, const Point4D&)> _addBonusCallBack;
    std::function<void(const std::string&)> _removeBonusCallBack;
public:
    explicit Client(std::shared_ptr<Player>  player) : _player(player){};

    void updatePacket() override;

    void setSpawnPlayerCallBack(std::function<void(sf::Uint16)> spawn);
    void setRemovePlayerCallBack(std::function<void(sf::Uint16)> remove);
    void setAddFireTraceCallBack(std::function<void(const Point4D&, const Point4D&)> addTrace);
    void setAddBonusCallBack(std::function<void(const std::string&, const Point4D&)> addBonus);
    void setRemoveBonusCallBack(std::function<void(const std::string&)> removeBonus);

    void processInit(sf::Packet& packet) override;
    void processUpdate(sf::Packet& packet) override;
    void processNewClient(sf::Packet& packet) override;
    void processDisconnect(sf::Uint16 targetId) override;

    void processCustomPacket(MsgType type, sf::Packet& packet) override;

    void processDisconnected() override;

    void damagePlayer(sf::Uint16 targetId, double damage);

    void takeBonus(const std::string& bonusName);

    void addTrace(const Point4D& from, const Point4D& to);

    void addPlayer(sf::Uint16 id, std::shared_ptr<Player> player);
};


#endif //MINECRAFT_3DZAVR_CLIENT_H
