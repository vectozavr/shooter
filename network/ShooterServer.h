//
// Created by Иван Ильин on 25.05.2021.
//

#ifndef SHOOTER_SHOOTERSERVER_H
#define SHOOTER_SHOOTERSERVER_H

#include "../engine/network/ServerUDP.h"
#include "../player/Player.h"

struct BonusInfo final {
    const Vec3D position{};
    const double lastTake = std::numeric_limits<double>::min();
    const bool onTheMap = false;
};

class ShooterServer final : public ServerUDP {
private:
    std::map<sf::Uint16, std::shared_ptr<Player>> _players{};
    std::map<std::string, std::shared_ptr<BonusInfo>> _bonuses{};
public:
    ShooterServer() = default;

    void broadcast() override;

    void processConnect(sf::Uint16 senderId) override;

    void processClientUpdate(sf::Uint16 senderId, sf::Packet &packet) override;

    void processDisconnect(sf::Uint16 senderId) override;

    void processCustomPacket(sf::Packet &packet, sf::Uint16 senderId) override;

    void processStop() override;

    void generateBonuses();

    void updateInfo() override;

    ~ShooterServer() override;
};


#endif //MINECRAFT_3DZAVR_SERVER_H
