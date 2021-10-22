//
// Created by Иван Ильин on 07.10.2021.
//

#ifndef SHOOTER_CONSTS_H
#define SHOOTER_CONSTS_H
#include <SFML/Graphics.hpp>


namespace Consts {
    const int STANDARD_SCREEN_WIDTH = 1920;
    const int STANDARD_SCREEN_HEIGHT = 1920;
    const sf::Color BACKGROUND_COLOR = sf::Color(255, 255, 255);
    const std::string PROJECT_NAME = "engine";
    const bool USE_LOG_FILE = true;
    const bool USE_OPEN_GL = true;

    const double PI = 3.14159265358979323846264338327950288;
    const double EPS = 0.000001;

    const double EPA_EPS = 0.0001;

    const double RAY_CAST_MAX_DISTANCE = 10000;

    const std::string THIN_FONT = "engine/fonts/Roboto-Thin.ttf";
    const std::string MEDIUM_FONT = "engine/fonts/Roboto-Medium.ttf";

    const double LARGEST_TIME_STEP = 1.0/15.0;
    const double TAP_DELAY = 0.2;

    const unsigned NETWORK_VERSION = 1U;
    const int NETWORK_TIMEOUT = 1U;
    const int NETWORK_WORLD_UPDATE_RATE = 30;
    const double NETWORK_RELIABLE_RETRY_TIME = 1.0/20;
    const uint16_t NETWORK_MAX_CLIENTS = 64;
}

#endif //SHOOTER_CONSTS_H
