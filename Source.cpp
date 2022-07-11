//
// Created by Ivan Ilin on 06.02.2021.
//

#include "Shooter.h"

using namespace std;


int main() {
    Shooter game;

    // Optimal for standard monitors:
    //game.create(720, 480, ShooterConsts::PROJECT_NAME, true);
    game.create(1280, 720, ShooterConsts::PROJECT_NAME, true);
    //game.create(1920, 1080, ShooterConsts::PROJECT_NAME, true, Consts::BACKGROUND_COLOR, sf::Style::Fullscreen);

    // Optimal for MacBook Pro 16 display:
    //game.create(2048, 1152, ShooterConsts::PROJECT_NAME, true, Consts::BACKGROUND_COLOR);
    //game.create(3072, 1920, ShooterConsts::PROJECT_NAME, true, Consts::BACKGROUND_COLOR, sf::Style::Fullscreen);

    return 0;
}