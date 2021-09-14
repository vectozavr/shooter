//
// Created by Иван Ильин on 25.05.2021.
//

#include "Client.h"
#include "utils/Log.h"
#include "Bonus.h"

void Client::updatePacket() {
    sf::Packet packet;
    packet << MsgType::ClientUpdate << _player->position().x() << _player->position().y() << _player->position().z() << _player->angle().y() << _player->camera()->angleLeftUpLookAt().x();
    _socket.send(packet, _socket.serverId());
}

void Client::spawnPlayer(sf::Uint16 id) {
    std::string name = "Player_" + std::to_string(id);
    _players.insert({ id, std::make_shared<Player>() });
    (*_world).addMesh(_players[id], name);
    _players[id]->setVisible(true);
    _players[id]->setAcceleration(Point4D{0, 0, 0});

    // add head and other stuff:
    _world->loadObj(name + "_head", "../obj/cube.obj", "",Point4D{0.7, 0.7, 0.7});
    (*_world)[name + "_head"]->translate(Point4D{0, 2, 0});
    (*_world)[name + "_head"]->setCollider(false);
    _players[id]->attach((*_world)[name + "_head"]);

    _world->loadObj(name + "_eye1", "../obj/cube.obj", "",Point4D{0.2, 0.2, 0.05});
    (*_world)[name + "_eye1"]->translate(Point4D{0.3, 2.1, 0.7});
    (*_world)[name + "_eye1"]->setCollider(false);
    (*_world)[name + "_eye1"]->setColor({147, 159, 255});
    (*_world)[name + "_head"]->attach((*_world)[name + "_eye1"]);

    _world->loadObj(name + "_eye2", "../obj/cube.obj", "",Point4D{0.2, 0.2, 0.05});
    (*_world)[name + "_eye2"]->translate(Point4D{-0.3, 2.1, 0.7});
    (*_world)[name + "_eye2"]->setCollider(false);
    (*_world)[name + "_eye2"]->setColor({147, 159, 255});
    (*_world)[name + "_head"]->attach((*_world)[name + "_eye2"]);
}

void Client::processInit(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[4];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3])
    {
        if(targetId != _socket.ownId()) {
            spawnPlayer(targetId);

            _players[targetId]->translateToPoint(Point4D{ buf[0], buf[1], buf[2]});
            _players[targetId]->setHealth(buf[3]);
        }
    }
}

void Client::processUpdate(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[6];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3] >> buf[4] >> buf[5]) {
        if (_players.count(targetId)) {
            std::string name = "Player_" + std::to_string(targetId);
            _players[targetId]->translateToPoint(Point4D{buf[0], buf[1], buf[2]});
            _players[targetId]->setHealth(buf[3]);
            _players[targetId]->rotateToAngle(Point4D{0, buf[4], 0});

            (*_world)[name + "_head"]->rotate(Matrix4x4::RotationY(buf[4]) * Point4D{1, 0, 0},
                                              buf[5] - _players[targetId]->headAngle());
            _players[targetId]->setHeadAngle(buf[5]);
        } else if (targetId == _socket.ownId()) {
            _player->setHealth(buf[3]);
        }
    }
}

void Client::processNewClient(sf::Packet& packet) {
    sf::Uint16 targetId;

    packet >> targetId;

    spawnPlayer(targetId);
}

void Client::processDisconnect(sf::Uint16 targetId) {
    if (targetId != _socket.ownId() && _players.count(targetId)) {
        std::string name = "Player_" + std::to_string(targetId);
        _world->removeMesh(name);
        _world->removeMesh(name + "_head");
        _world->removeMesh(name + "_eye1");
        _world->removeMesh(name + "_eye2");

        _players.erase(targetId);
    }
}


void Client::processCustomPacket(MsgType type, sf::Packet& packet) {
    int buff[3];
    sf::Uint16 buffId[2];
    double dbuff[10];
    std::string tmp, tmp2;
    Point4D p1, p2;

    switch (type) {
        case MsgType::Kill:
            packet >> buffId[0] >> buffId[1];
            if(buffId[0] == _socket.ownId()) {
                _player->addDeath();
                // respawn
                _player->translateToPoint(Point4D{50.0*(-1 + 2.0*(double)rand()/RAND_MAX),30.0*(double)rand()/RAND_MAX,50.0*(-1 + 2.0*(double)rand()/RAND_MAX)});
                _player->playDeath();
                _player->initWeapons();
                _player->setFullAbility();
            }
            else
                _players[buffId[0]]->addDeath();
            if(buffId[1] == _socket.ownId()) {
                _player->addKill();
                _player->playKill();
            }
            else
                _players[buffId[1]]->addKill();
            break;
        case MsgType::FireTrace:
            packet >> dbuff[0] >> dbuff[1] >> dbuff[2] >> dbuff[3] >> dbuff[4] >> dbuff[5];

            p1 = Point4D(dbuff[0], dbuff[1], dbuff[2]);
            p2 = Point4D(dbuff[3], dbuff[4], dbuff[5]);

            tmp = "Client_fireTraces_" + std::to_string(fireTraces++);
            _world->addMesh(std::make_shared<Mesh>(Mesh::LineTo(p1, p2, 0.05)), tmp);
            (*_world)[tmp]->setCollider(false);

            (*_world)[tmp]->a_color(tmp + "_fadeOut", {255, 255, 255, 0}, 1, Animation::None, Animation::linear);
            (*_world)["Player_im"]->a_function(tmp + "delete", [this, tmp](){ deleteTrace(_world, tmp); }, 1, 2);

            break;
        case MsgType::InitBonuses:
            while (packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2]) {
                tmp2 = tmp.substr(6, tmp.size()-3-5);
                _world->addMesh(std::make_shared<Bonus>(tmp, "../obj/" + tmp2 + ".obj", "../obj/" + tmp2 + "_mat.txt", Point4D{3, 3, 3}), tmp);
                (*_world)[tmp]->translateToPoint(Point4D(dbuff[0], dbuff[1], dbuff[2]));
            }
            break;

        case MsgType::AddBonus:
            packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2];

            tmp2 = tmp.substr(6, tmp.size()-3-5);
            _world->addMesh(std::make_shared<Bonus>(tmp, "../obj/" + tmp2 + ".obj", "../obj/" + tmp2 + "_mat.txt", Point4D{3, 3, 3}), tmp);
            (*_world)[tmp]->translateToPoint(Point4D(dbuff[0], dbuff[1], dbuff[2]));

            break;
        case MsgType::RemoveBonus:
            packet >> tmp;
            _world->removeMesh(tmp);
            break;
    }
}

void Client::processDisconnected() {
    for (auto it = _players.begin(); it != _players.end();) {
        std::string name = "Player_" + std::to_string(it->first);

        _world->removeMesh(name);
        _world->removeMesh(name + "_head");
        _world->removeMesh(name + "_eye1");
        _world->removeMesh(name + "_eye2");
        _players.erase(it++);
    }
}

void Client::damagePlayer(sf::Uint16 targetId, double damage) {
    sf::Packet packet;

    packet << MsgType::Damage << targetId << damage;
    _socket.send(packet, _socket.serverId());

    Log::log("Client: damagePlayer " + std::to_string(targetId) + " ( -" + std::to_string(damage) + "hp )");
}

void Client::addTrace(const Point4D& from, const Point4D& to) {
    sf::Packet packet;

    packet << MsgType::FireTrace << from.x() << from.y() << from.z() << to.x() << to.y() << to.z();
    _socket.send(packet, _socket.serverId());
}

void Client::deleteTrace(std::shared_ptr<World> world, const std::string &traceName) {
    world->removeMesh(traceName);
}

void Client::takeBonus(const std::string& bonusName) {
    sf::Packet packet;

    packet << MsgType::RemoveBonus << bonusName;
    _socket.send(packet, _socket.serverId());
}
