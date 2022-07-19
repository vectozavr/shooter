//
// Created by Иван Ильин on 19.09.2021.
//

#ifndef SHOOTER_PLAYERCONTROLLER_H
#define SHOOTER_PLAYERCONTROLLER_H

#include "Player.h"
#include "../engine/io/Keyboard.h"
#include "../engine/io/Mouse.h"

class PlayerController final {
private:
    std::shared_ptr<Player> _player;
    std::shared_ptr<Keyboard> _keyboard;
    std::shared_ptr<Mouse> _mouse;

    bool _inRunning = false;
    bool _isSliding = false;
    bool _isInSlowMo = false;

public:
    PlayerController(std::shared_ptr<Player> player, std::shared_ptr<Keyboard> keyboard, std::shared_ptr<Mouse> mouse);

    void update();
};


#endif //SHOOTER_PLAYERCONTROLLER_H
