//
// Created by Ivan Ilin on 06.02.2021.
//

#include "Shooter.h"

using namespace std;

int main() {
    Shooter game;

    game.create(1280, 720, "Shooter");
    //game.create(1920, 1080, "Shooter", true, {255, 255, 255}, sf::Style::Fullscreen);

    //game.create(2048, 1152, "Shooter");
    //game.create(3072, 1920, "Shooter", true, {255, 255, 255}, sf::Style::Fullscreen);

    return 0;
}