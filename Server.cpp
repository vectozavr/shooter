//
// Created by Иван Ильин on 22.09.2021.
//

#include <fstream>
#include <utility>
#include <iostream>
#include "network/ShooterServer.h"
#include <utils/Time.h>
#include <utils/Log.h>
#include <Consts.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

void InitServer(ShooterServer& server)
{
    sf::Uint16 serverPort;
    std::ifstream connectFile("server.txt", std::ifstream::in);

    if (!connectFile.is_open() || !(connectFile >> serverPort))
    {
        connectFile.close();
        // Create file and write default settings
        serverPort = 54000;
        std::ofstream temp("server.txt", std::ofstream::out);
        temp << serverPort;
        temp.close();
    }
    connectFile.close();

    server.start(serverPort);
    if(server.isWorking())
        server.generateBonuses();
}

int main() {
    Time::init();
    ShooterServer server{};

    Log::log("Initializing the server...");
    InitServer(server);

    double lastTryReconnecting = 0;

    while (true) {
        if(!server.isWorking() && (Time::time() - lastTryReconnecting > Consts::NETWORK_TIMEOUT)) {
            lastTryReconnecting = Time::time();
            server.stop();
            Log::log("Restarting the server...");
            InitServer(server);
        }
        Time::update();
        server.update();

        sleep_for(10ms);
    }

    return 0;
}
