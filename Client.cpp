//
// Created by Иван Ильин on 25.05.2021.
//

#include "Client.h"

#include <utility>
#include "engine/utils/Log.h"
#include "Bonus.h"

void Client::updatePacket() {
    sf::Packet packet;
    packet << MsgType::ClientUpdate << _player->position().x() << _player->position().y() << _player->position().z() << _player->angle().y() << _player->headAngle();
    _socket.send(packet, _socket.serverId());
}

void Client::processInit(sf::Packet& packet) {
    sf::Uint16 targetId;
    double buf[4];

    while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3])
    {
        if(targetId != _socket.ownId()) {
            if(_spawnPlayerCallBack != nullptr)
                _spawnPlayerCallBack(targetId);

            _players[targetId]->translateToPoint(Vec3D{ buf[0], buf[1], buf[2]});
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
            _players[targetId]->translateToPoint(Vec3D{buf[0], buf[1], buf[2]});
            _players[targetId]->setHealth(buf[3]);
            _players[targetId]->rotateToAngle(Vec3D{0, buf[4], 0});

            auto head =  _players[targetId]->attached(ObjectNameTag("head"));
            auto weapon = _players[targetId]->attached(ObjectNameTag("Weapon"));

            head->rotateLeft(buf[5] - _players[targetId]->headAngle());
            weapon->rotateLeft(-(buf[5] - _players[targetId]->headAngle()));

            _players[targetId]->setHeadAngle(buf[5]);
        } else if (targetId == _socket.ownId()) {
            _player->setHealth(buf[3]);
        }
    }
}

void Client::processNewClient(sf::Packet& packet) {
    sf::Uint16 targetId;

    packet >> targetId;

    if(_spawnPlayerCallBack != nullptr)
        _spawnPlayerCallBack(targetId);
}

void Client::processDisconnect(sf::Uint16 targetId) {
    if (targetId != _socket.ownId() && _players.count(targetId)) {
        _players.erase(targetId);
        _removePlayerCallBack(targetId);
    }
}


void Client::processCustomPacket(MsgType type, sf::Packet& packet) {
    sf::Uint16 buffId[2];
    double dbuff[10];
    std::string tmp, tmp2;

    switch (type) {
        case MsgType::Kill:
            packet >> buffId[0] >> buffId[1];
            if(buffId[0] == _socket.ownId()) {
                _player->addDeath();
                // respawn
                _player->translateToPoint(Vec3D{50.0*(-1 + 2.0*(double)rand()/RAND_MAX),30.0*(double)rand()/RAND_MAX,50.0*(-1 + 2.0*(double)rand()/RAND_MAX)});
                _player->initWeapons();
                _player->setFullAbility();
                SoundController::playSound(SoundTag("death"), ShooterConsts::DEATH_SOUND);
            }
            else
                _players[buffId[0]]->addDeath();
            if(buffId[1] == _socket.ownId()) {
                _player->addKill();
                SoundController::playSound(SoundTag("kill"), ShooterConsts::KILL_SOUND);
            }
            else
                _players[buffId[1]]->addKill();
            break;
        case MsgType::FireTrace:
            packet >> dbuff[0] >> dbuff[1] >> dbuff[2] >> dbuff[3] >> dbuff[4] >> dbuff[5];

            if(_addFireTraceCallBack != nullptr)
                _addFireTraceCallBack(Vec3D(dbuff[0], dbuff[1], dbuff[2]), Vec3D(dbuff[3], dbuff[4], dbuff[5]));

            break;
        case MsgType::InitBonuses:
            while (packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2]) {
                if(_addBonusCallBack != nullptr)
                    _addBonusCallBack(tmp, Vec3D(dbuff[0], dbuff[1], dbuff[2]));
            }
            break;

        case MsgType::AddBonus:
            packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2];
            if(_addBonusCallBack != nullptr)
                _addBonusCallBack(tmp, Vec3D(dbuff[0], dbuff[1], dbuff[2]));

            break;
        case MsgType::RemoveBonus:
            packet >> tmp;
            if(_removeBonusCallBack != nullptr)
                _removeBonusCallBack(ObjectNameTag(tmp));
            break;
        case MsgType::ChangeWeapon:
            packet >> buffId[0] >> tmp;

            if(_changeEnemyWeaponCallBack != nullptr)
                _changeEnemyWeaponCallBack(tmp, buffId[0]);
            break;
        default:
            return;
    }
}

void Client::processDisconnected() {
    for (auto it = _players.begin(); it != _players.end();) {
        processDisconnect(it++->first);
    }
}

void Client::damagePlayer(sf::Uint16 targetId, double damage) {
    sf::Packet packet;

    packet << MsgType::Damage << targetId << damage;
    _socket.sendRely(packet, _socket.serverId());

    Log::log("Client: damagePlayer " + std::to_string(targetId) + " ( -" + std::to_string(damage) + "hp )");
}

void Client::addTrace(const Vec3D& from, const Vec3D& to) {
    sf::Packet packet;

    packet << MsgType::FireTrace << from.x() << from.y() << from.z() << to.x() << to.y() << to.z();
    _socket.send(packet, _socket.serverId());
}

void Client::takeBonus(const std::string& bonusName) {
    sf::Packet packet;

    packet << MsgType::RemoveBonus << bonusName;
    _socket.sendRely(packet, _socket.serverId());

    if(_removeBonusCallBack != nullptr)
        _removeBonusCallBack(ObjectNameTag(bonusName));
}

void Client::changeWeapon(const std::string &weaponName) {
    sf::Packet packet;

    packet << MsgType::ChangeWeapon << weaponName;
    _socket.sendRely(packet, _socket.serverId());
}

void Client::addPlayer(sf::Uint16 id, std::shared_ptr<Player> player) {
    _players.insert({ id, player });
}

void Client::setSpawnPlayerCallBack(std::function<void(sf::Uint16)> spawn) {
    _spawnPlayerCallBack = std::move(spawn);
}

void Client::setRemovePlayerCallBack(std::function<void(sf::Uint16)> remove) {
    _removePlayerCallBack = std::move(remove);
}

void Client::setAddFireTraceCallBack(std::function<void(const Vec3D &, const Vec3D &)> addTrace) {
    _addFireTraceCallBack = std::move(addTrace);
}

void Client::setAddBonusCallBack(std::function<void(const std::string &, const Vec3D &)> addBonus) {
    _addBonusCallBack = std::move(addBonus);
}

void Client::setRemoveBonusCallBack(std::function<void(const ObjectNameTag &)> removeBonus) {
    _removeBonusCallBack = std::move(removeBonus);
}

void Client::setChangeEnemyWeaponCallBack(std::function<void(const std::string&, sf::Uint16)> changeEnemyWeapon) {
    _changeEnemyWeaponCallBack = std::move(changeEnemyWeapon);
}
