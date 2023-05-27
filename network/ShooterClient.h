//
// Created by Иван Ильин on 25.05.2021.
//

#ifndef SHOOTER_SHOOTERCLIENT_H
#define SHOOTER_SHOOTERCLIENT_H

#include "../3dzavr/engine/network/ClientUDP.h"
#include "../player/Player.h"
#include <SFML/Config.hpp>
#include "Chat.h"

class ShooterClient final : public ClientUDP {
private:
    std::string _lastEvent;

    std::map<sf::Uint16, std::shared_ptr<Player>> _players{};
    std::shared_ptr<Player> _player;

    std::shared_ptr<ChatManager> chatManager;

    void damagePlayer(sf::Uint16 targetId, double damage);

    void takeBonus(const std::string &bonusName);

    void sendTrace(const Vec3D &from, const Vec3D &to);

    void changeWeapon(const std::string &weaponName);

public:
    explicit ShooterClient(std::shared_ptr<Player> player);

    void sendMessage(std::string message);

    void sendChatMessage(std::string message, std::string name);

    void updatePacket() override;

    void processInit(sf::Packet &packet) override;

    void processUpdate(sf::Packet &packet) override;

    void processNewClient(sf::Packet &packet) override;

    void processDisconnect(sf::Uint16 targetId) override;

    void processCustomPacket(sf::Packet &packet) override;

    void processDisconnected() override;

    void setChatManager(std::shared_ptr<ChatManager> chat) { chatManager = chat; };

    void addPlayer(sf::Uint16 id, std::shared_ptr<Player> player);

    static void requestMap(const std::string& clientIp, std::string *current_map);

    [[nodiscard]] std::map<sf::Uint16, std::shared_ptr<Player>> const &players() const { return _players; }

    [[nodiscard]] std::string lastEvent() const { return _lastEvent; }
};


#endif //MINECRAFT_3DZAVR_CLIENT_H
