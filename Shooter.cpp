//
// Created by Иван Ильин on 22.09.2021.
//

#include "Shooter.h"
#include <fstream>
#include "engine/animation/AColor.h"
#include "engine/animation/AFunction.h"
#include "engine/animation/ARotate.h"
#include "engine/animation/Timeline.h"

using namespace std;

// Read server/client settings and start both.
// If client doesn't connect to the localhost - server doesn't start.
void Shooter::InitNetwork()
{
    std::string clientIp;
    sf::Uint16 clientPort;
    sf::Uint16 serverPort;
    std::ifstream connectFile("connect.txt", std::ifstream::in);

    // If failed to read client settings
    if (!connectFile.is_open() || !(connectFile >> clientIp >> clientPort) || sf::IpAddress(clientIp) == sf::IpAddress::None)
    {
        connectFile.close();
        // Create file and write default settings
        clientIp = "127.0.0.1";
        clientPort = 54000;
        std::ofstream temp("connect.txt", std::ofstream::out);
        temp << clientIp << std::endl << clientPort;
        temp.close();
    }
    connectFile.close();

    // If failed to read server settings
    connectFile.open("server.txt", std::ifstream::in);
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

    if (clientIp == sf::IpAddress::LocalHost) {
        server->start(serverPort);
        if(server->isWorking())
            server->generateBonuses();
    }

    client->connect(clientIp, clientPort);

    client->setSpawnPlayerCallBack([this](sf::Uint16 id){ spawnPlayer(id); });
    client->setRemovePlayerCallBack([this](sf::Uint16 id){ removePlayer(id); });
    client->setAddFireTraceCallBack([this](const Vec3D& from, const Vec3D& to){ addFireTrace(from, to); });
    client->setAddBonusCallBack([this](const std::string& bonusName, const Vec3D& position){ addBonus(bonusName, position); });
    client->setRemoveBonusCallBack([this](const std::string& bonusName){ removeBonus(bonusName); });
}

void Shooter::start() {
    // This code executed once in the beginning:
    //setDebugText(false);
    setUpdateWorld(false);

    mouse->setMouseCursorVisible(true);

    world->loadMap("maps/map1.obj", "maps/materials.txt", "map", Vec3D{5, 5, 5});

    player = std::make_shared<Player>();
    playerController = std::make_shared<PlayerController>(player, keyboard, mouse);

    player->setAddTraceCallBack([this](const Vec3D& from, const Vec3D& to){ client->addTrace(from, to); addFireTrace(from, to); });
    player->setDamagePlayerCallBack([this] (sf::Uint16 targetId, double damage) { client->damagePlayer(targetId, damage); });
    player->setRayCastFunction([this](const Vec3D& from, const Vec3D& to) { return world->rayCast(from, to); });
    player->setTakeBonusCallBack([this] (const string& bonusName) { client->takeBonus(bonusName); });
    player->setAddWeaponCallBack([this](std::shared_ptr<Weapon> weapon){ addWeapon(weapon); });
    player->setRemoveWeaponCallBack([this](std::shared_ptr<Weapon> weapon){ removeWeapon(weapon); });

    player->initWeapons();

    camera->translateToPoint(player->position() + Vec3D{0, 1.8, 0});
    player->attach(camera, "camera");

    world->addBody(player, "Player");
    player->translate(Vec3D{0, 10, 0});

    client = std::make_shared<Client>(player);
    server = std::make_shared<Server>();

    // windows init:
    mainMenu.title("Main menu");
    mainMenu.setBackgroundTexture("textures/back.png", 1.1, 1.1, screen->width(), screen->height());

    mainMenu.addButton(screen->width()/2, 200, 200, 20, [this] () { this->play(); }, "Play", 5, 5, "textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "sound/click.ogg");
    mainMenu.addButton(screen->width()/2, 350, 200, 20, [this] () { this->player->translateToPoint(Vec3D{0, 0, 0}); this->player->setVelocity({}); this->play(); }, "Respawn", 5, 5, "textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "sound/click.ogg");

    mainMenu.addButton(screen->width()/2, 500, 200, 20, [this] () { client->disconnect(); server->stop(); this->exit();}, "Exit", 5, 5, "textures/gui.png", {0, 66}, {0, 86}, {0, 46}, "engine/fonts/Roboto-Medium.ttf", {255, 255, 255}, "sound/click.ogg");

    // connecting to the server
    InitNetwork();
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

void Shooter::update() {
    // This code executed every time step:

    server->update();
    client->update();

    // Check all input after this condition please
    if (!screen->hasFocus())
        return;

    if(keyboard->isKeyTapped(sf::Keyboard::Escape)) {
        inGame = !inGame;
        mouse->setMouseCursorVisible(!inGame);
    }

    if(inGame) {
        screen->setTitle("Shooter");
        playerController->update();
        mouse->setMouseInCenter();
        camera->rotateLeft(player->headAngle() - camera->angleLeftUpLookAt().x());

    } else {
        mainMenu.update();
    }

    setUpdateWorld(inGame);

    // background sounds and music control
    if(backgroundNoise.getStatus() != sf::Sound::Status::Playing) {
        backgroundNoise.setBuffer(*ResourceManager::loadSoundBuffer("sound/backNoise.ogg"));
        backgroundNoise.play();
    }
}

void Shooter::gui() {

    sf::Sprite sprite;
    sprite.setTexture(*ResourceManager::loadTexture("textures/gui.png"));
    sprite.setTextureRect(sf::IntRect(243, 3, 9, 9));
    sprite.scale(3, 3);
    sprite.setPosition(screen->width() / 2.0f - 27.0f/2.0f, screen->height() / 2.0f - 27.0f/2.0f);
    sprite.setColor(sf::Color(0, 0, 0, 200));
    screen->drawSprite(sprite);

    // health player stats
    drawPlayerStats();
}

void Shooter::drawPlayerStats() {
    // health bar
    double xPos = 10;
    double yPos = screen->height() - 20;

    int width = screen->width()/2 - 20;
    int height = 10;

    screen->drawTetragon(Vec2D{xPos, yPos},
                         Vec2D{xPos + width * player->health() / player->maxHealth(), yPos},
                         Vec2D{xPos + width * player->health() / player->maxHealth(), yPos + height},
                         Vec2D{xPos, yPos + height},
                         { static_cast<sf::Uint8>((player->maxHealth() - player->health())/player->maxHealth() * 255), static_cast<sf::Uint8>(player->health() * 255 / player->maxHealth()), 0, 100 });

    screen->drawTetragon(Vec2D{xPos, yPos - 15},
                         Vec2D{xPos + width * player->ability() / player->maxAbility(), yPos - 15},
                         Vec2D{xPos + width * player->ability() / player->maxAbility(), yPos - 15 + height},
                         Vec2D{xPos, yPos - 15 + height},
                         { 255, 168, 168, 100 });

    auto balance = player->balance();

    screen->drawText(std::to_string((int)balance.first), Vec2D{150, static_cast<double>(screen->height() - 150)}, 100, sf::Color(0, 0, 0, 100));
    screen->drawText(std::to_string((int)balance.second), Vec2D{50, static_cast<double>(screen->height() - 100)}, 50, sf::Color(0, 0, 0, 70));

    screen->drawText("KILLS: " + std::to_string(player->kills()) + " | " + "DEATHS: " + std::to_string(player->deaths()),
                     Vec2D{10, 10},25, sf::Color(0, 0, 0, 100));
}

void Shooter::play() {
    inGame = true;
    mouse->setMouseCursorVisible(false);
}

void Shooter::spawnPlayer(sf::Uint16 id) {
    std::string name = "Enemy_" + std::to_string(id);
    std::shared_ptr<Player> newPlayer = std::make_shared<Player>();
    newPlayer->setCollision(false);

    client->addPlayer(id, newPlayer);
    world->addBody(newPlayer, name);
    newPlayer->setVisible(true);
    newPlayer->setAcceleration(Vec3D{0, 0, 0});

    // add head and other stuff:
    world->loadBody(name + "_head", "obj/cube.obj", "", Vec3D{0.7, 0.7, 0.7});
    world->body(name + "_head")->translate(Vec3D{0, 2, 0});
    world->body(name + "_head")->setCollider(false);
    newPlayer->attach(world->body(name + "_head"), "head");

    world->loadBody(name + "_eye1", "obj/cube.obj", "", Vec3D{0.2, 0.2, 0.05});
    world->body(name + "_eye1")->translate(Vec3D{0.3, 2.1, 0.7});
    world->body(name + "_eye1")->setCollider(false);
    world->body(name + "_eye1")->setColor({147, 159, 255});
    world->body(name + "_head")->attach(world->body(name + "_eye1"), "eye1");

    world->loadBody(name + "_eye2", "obj/cube.obj", "", Vec3D{0.2, 0.2, 0.05});
    world->body(name + "_eye2")->translate(Vec3D{-0.3, 2.1, 0.7});
    world->body(name + "_eye2")->setCollider(false);
    world->body(name + "_eye2")->setColor({147, 159, 255});
    world->body(name + "_head")->attach(world->body(name + "_eye2"), "eye2");
}

void Shooter::removePlayer(sf::Uint16 id) {
    std::string name = "Enemy_" + std::to_string(id);
    world->removeBody(name);
    world->removeBody(name + "_head");
    world->removeBody(name + "_eye1");
    world->removeBody(name + "_eye2");
}

void Shooter::addFireTrace(const Vec3D &from, const Vec3D &to) {
    std::string traceName = "Client_fireTraces_" + std::to_string(fireTraces++);
    world->addBody(std::make_shared<RigidBody>(Mesh::LineTo(from, to, 0.05)), traceName);
    world->body(traceName)->setCollider(false);

    Timeline::animate(traceName + "_fadeOut", new AColor(world->body(traceName), {255, 255, 255, 0}));
    Timeline::animate(traceName + "_delete", new AFunction([this, traceName](){ deleteFireTrace(traceName); }, 1, 2));
}

void Shooter::deleteFireTrace(const std::string& traceName) {
    world->removeBody(traceName);
}

void Shooter::addBonus(const string &bonusName, const Vec3D &position) {
    std::string name = bonusName.substr(6, bonusName.size()-3-5);
    world->addBody(std::make_shared<Bonus>(bonusName, "obj/" + name + ".obj", "obj/" + name + "_mat.txt", Vec3D{3, 3, 3}), bonusName);
    world->body(bonusName)->translateToPoint(position);
    Timeline::animate(bonusName + "_rotation", new ARotate(world->body(bonusName), Vec3D{0, 2*Consts::PI, 0}, 4, Animation::LoopOut::Continue, Animation::InterpolationType::linear));
}

void Shooter::removeBonus(const string &bonusName) {
    world->removeBody(bonusName);
}

void Shooter::addWeapon(std::shared_ptr<Weapon> weapon) {
    world->addBody(weapon, weapon->name());
}

void Shooter::removeWeapon(std::shared_ptr<Weapon> weapon) {
    world->removeBody(weapon->name());
}
