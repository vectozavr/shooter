//
// Created by Иван Ильин on 19.09.2021.
//

#ifndef SHOOTER_PLAYERCONTROLLER_H
#define SHOOTER_PLAYERCONTROLLER_H

#include "Player.h"
#include "engine/Keyboard.h"
#include "engine/Mouse.h"

class PlayerController {
private:
    std::shared_ptr<Player> _player;
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Mouse> _mouse;

    bool _inRunning = false;
    bool _isSliding = false;
    double _slowMoCoefficient = 5;
    bool _isInSlowMo = false;

    sf::Sound _slowMoSound;
    sf::Sound _unSlowMoSound;
    sf::Sound _walkSound;

    double _jumpHeight = 3;
    double _walkSpeed = 10;
public:
    PlayerController(std::shared_ptr<Player> player, std::shared_ptr<Keyboard> keyboard, std::shared_ptr<Mouse> mouse);
    void update();
};


#endif //SHOOTER_PLAYERCONTROLLER_H
