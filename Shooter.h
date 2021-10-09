//
// Created by Иван Ильин on 22.09.2021.
//

#ifndef SHOOTER_SHOOTER_H
#define SHOOTER_SHOOTER_H

#include "engine/Engine.h"
#include "Player.h"
#include "PlayerController.h"
#include "PlayerController.h"
#include "engine/gui/Window.h"

#include "Client.h"
#include "Server.h"


class Shooter : public Engine {
private:
    std::shared_ptr<Player> player;
    std::shared_ptr<PlayerController> playerController;

    sf::Sound backgroundNoise;

    Window mainMenu;

    std::shared_ptr<Server> server;
    std::shared_ptr<Client> client;

    bool inGame = false;

    int fireTraces = 0;

    void start() override;
    void update() override;

    void gui() override;

    void play();

    void drawPlayerStats();
    void InitNetwork();

    void spawnPlayer(sf::Uint16 id);
    void removePlayer(sf::Uint16 id);
    void addFireTrace(const Point4D& from, const Point4D& to);
    void deleteFireTrace(const std::string& traceName);
    void addBonus(const std::string& bonusName, const Point4D& position);
    void removeBonus(const std::string& bonusName);
    void addWeapon(std::shared_ptr<Weapon> weapon);
    void removeWeapon(std::shared_ptr<Weapon> weapon);
public:
    Shooter() : mainMenu(screen, mouse) {};
};


#endif //SHOOTER_SHOOTER_H
