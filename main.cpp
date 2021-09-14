//
// Created by Иван Ильин on 06.02.2021.
//

#include "Engine.h"
#include <iostream>
#include "Player.h"
#include "ResourceManager.h"
#include "gui/Window.h"

#include "Client.h"
#include "Server.h"
#include <fstream>

using namespace std;

// Read server/client settings and start both.
// If client doesn't connect to the localhost - server doesn't start.
void InitNetwork(shared_ptr<Server> server, shared_ptr<Client> client)
{
    std::string clientIp;
    sf::Uint16 clientPort;
    sf::Uint16 serverPort;
    std::ifstream connectfile("connect.txt", std::ifstream::in);

    // If failed to read client settings
    if (!connectfile.is_open() || !(connectfile >> clientIp >> clientPort) || sf::IpAddress(clientIp) == sf::IpAddress::None)
    {
        connectfile.close();
        // Create file and write default settings
        clientIp = "127.0.0.1";
        clientPort = 54000;
        std::ofstream temp("connect.txt", std::ofstream::out);
        temp << clientIp << std::endl << clientPort;
        temp.close();
    }
    connectfile.close();

    // If failed to read server settings
    connectfile.open("server.txt", std::ifstream::in);
    if (!connectfile.is_open() || !(connectfile >> serverPort))
    {
        connectfile.close();
        // Create file and write default settings
        serverPort = 54000;
        std::ofstream temp("server.txt", std::ofstream::out);
        temp << serverPort;
        temp.close();
    }
    connectfile.close();

    if (clientIp == sf::IpAddress::LocalHost) {
        server->start(serverPort);
        if(server->isWorking())
            server->generateBonuses();
    }

    client->connect(clientIp, clientPort);
}

class Shooter : public Engine {
private:
    shared_ptr<Player> player;

    sf::Sound backgroundNoise;

    Window mainMenu;

    shared_ptr<Server> server;
    shared_ptr<Client> client;

    bool inGame = false;

public:
    Shooter() = default;

    void start() override;
    void update(double elapsedTime) override;

    void gui() override;

    void play();
};

void Shooter::start() {
    // This code executed once in the beginning:
    debugText(false);

    screen->setMouseCursorVisible(true);

    world->loadMap("../maps/map1.obj", "map", Point4D{5, 5, 5});

    player = std::make_shared<Player>();
    client = std::make_shared<Client>(player, world);
    server = std::make_shared<Server>();

    player->setAddTraceCallBack([this](const Point4D& from, const Point4D& to){ client->addTrace(from, to); });
    player->attachCamera(camera, screen);
    player->attachWorld(world);

    setUpdateWorld(false);

    world->addMesh(player, player->name());

    player->setDamagePlayerCallBack([this] (sf::Uint16 targetId, double damage) { client->damagePlayer(targetId, damage); });
    player->setTakeBonusCallBack([this] (const string& bonusName) { client->takeBonus(bonusName); });


    // windows init:
    mainMenu.title("Main menu");
    mainMenu.setBackgroundTexture("../textures/back.png", 1.1, 1.1, screen->width(), screen->height());

    mainMenu.addButton(screen->width()/2, 200, 200, 20, [this] () { this->play(); }, "Play", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");
    mainMenu.addButton(screen->width()/2, 350, 200, 20, [this] () { this->player->translateToPoint(Point4D{0, 0, 0}); this->player->setVelocity({}); this->play(); }, "Respawn", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");

    mainMenu.addButton(screen->width()/2, 500, 200, 20, [this] () { client->disconnect(); server->stop(); this->exit();}, "Exit", 5, 5, "../textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "../engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "../sound/click.ogg");

    // connecting to the server
    InitNetwork(server, client);
    // Waiting for connect and updating server if it's same window
    while (client->isWorking() && !client->connected())
    {
        client->update();
        server->update();
        Time::update();
    }
    // If connect fail - return to menu
    if (!client->isWorking())
    {
        inGame = false;
        server->stop();
    }
}

void Shooter::update(double elapsedTime) {
    // This code executed every time step:

    server->update();
    client->update();

    // Check all input after this condition please
    if (!screen->window.hasFocus())
        return;

    if(screen->isKeyTapped(sf::Keyboard::Escape)) {
        inGame = !inGame;
        screen->setMouseCursorVisible(!inGame);
    }

    if(inGame) {
        screen->title("Shooter");
        player->update();
    } else {
        mainMenu.update(screen);
    }


    setUpdateWorld(inGame);

    // background sounds and music control
    if(backgroundNoise.getStatus() != sf::Sound::Status::Playing) {
        backgroundNoise.setBuffer(*ResourceManager::loadSoundBuffer("../sound/backNoise.ogg"));
        backgroundNoise.play();
    }
}

void Shooter::gui() {

    if(inGame) {
        // aim
        sf::Sprite sprite;
        sprite.setTexture(*ResourceManager::loadTexture("../textures/gui.png"));
        sprite.setTextureRect(sf::IntRect(243, 3, 9, 9));
        sprite.scale(3, 3);
        sprite.setPosition(screen->width() / 2.0 - 27.0/2.0, screen->height() / 2 - 27.0/2.0);
        sprite.setColor(sf::Color(0,0,0, 200));
        screen->window.draw(sprite);

        // health player stats
        player->drawStats();
    }
}

void Shooter::play() {
    inGame = true;

    screen->setMouseCursorVisible(!inGame);
}

int main(int argc, char* argv[]) {
    Shooter game;

    //game.create(1280, 720, "Shooter");
    //game.create(1920, 1080, "Shooter", true, {255, 255, 255}, sf::Style::Fullscreen);

    game.create(2048, 1152, "Shooter");
    //game.create(3072, 1920, "Shooter", true, {255, 255, 255}, sf::Style::Fullscreen);

    return 0;
}