//
// Created by Иван Ильин on 25.05.2021.
//

#include "ShooterClient.h"

#include <utility>
#include "../engine/utils/Log.h"
#include "../engine/animation/Timeline.h"
#include "ShooterMsgType.h"
#include "../engine/animation/Animations.h"

void ShooterClient::updatePacket() {
    sf::Packet packet;
    packet << MsgType::ClientUpdate << _player->position().x() << _player->position().y() << _player->position().z()
           << _player->angle().y() << _player->headAngle() << _player->playerNickName();
    _socket.send(packet, _socket.serverId());
}

void ShooterClient::processInit(sf::Packet &packet) {
    sf::Uint16 targetId;
    double x, y, z, health;
    int kills, deaths;

    while (packet >> targetId >> x >> y >> z >> health >> kills >> deaths) {
        if (targetId != _socket.ownId()) {
            if (_spawnPlayerCallBack != nullptr) {
                _spawnPlayerCallBack(targetId);
            }

            _players[targetId]->translateToPoint(Vec3D{x, y, z});
            _players[targetId]->setHealth(health);
            _players[targetId]->setKills(kills);
            _players[targetId]->setDeaths(deaths);
        }
    }
}

void ShooterClient::processUpdate(sf::Packet &packet) {
    sf::Uint16 targetId;
    double x, y, z, health, bodyAngle, headAngle;
    std::string playerName;

    while (packet >> targetId >> x >> y >> z >> health >> bodyAngle >> headAngle >> playerName) {
        if (_players.count(targetId)) {
            std::string name = "Enemy_" + std::to_string(targetId);

            Vec3D newPosition = Vec3D{x, y, z};

            bool isAnimate = (_players[targetId]->position() - newPosition).sqrAbs() > 0.2;

            _players[targetId]->translateToPoint(newPosition);

            _players[targetId]->setHealth(health);
            _players[targetId]->rotateToAngle(Vec3D{0, bodyAngle, 0});
            _players[targetId]->setPlayerNickName(playerName);

            auto head = _players[targetId]->attached(ObjectNameTag(name + "_head"));
            auto weapon = _players[targetId]->attached(ObjectNameTag("Enemy_" + std::to_string(targetId) + "_weapon"));

            auto foot1 = _players[targetId]->attached(ObjectNameTag(name + "_foot_1"));
            auto foot2 = _players[targetId]->attached(ObjectNameTag(name + "_foot_2"));

            if (head != nullptr) {
                head->rotateLeft(headAngle - _players[targetId]->headAngle());
            }
            if (weapon != nullptr) {
                weapon->rotateLeft(headAngle - _players[targetId]->headAngle());
            }

            if (isAnimate) {
                if (foot1 != nullptr && foot2 != nullptr &&
                    !Timeline::isInAnimList(AnimationListTag(name + "_foot1_rotation"))) {
                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot1_rotation"),
                                                        foot1, 0.6, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);
                    Timeline::addAnimation<AWait>(AnimationListTag(name + "_foot1_rotation"), 0);
                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot1_rotation"),
                                                        foot1, -1.2, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);
                    Timeline::addAnimation<AWait>(AnimationListTag(name + "_foot1_rotation"), 0);
                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot1_rotation"),
                                                        foot1, 0.6, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);

                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot2_rotation"),
                                                        foot2, -0.6, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);
                    Timeline::addAnimation<AWait>(AnimationListTag(name + "_foot2_rotation"), 0);
                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot2_rotation"),
                                                        foot2, 1.2, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);
                    Timeline::addAnimation<AWait>(AnimationListTag(name + "_foot2_rotation"), 0);
                    Timeline::addAnimation<ARotateLeft>(AnimationListTag(name + "_foot2_rotation"),
                                                        foot2, -0.6, 0.2, Animation::LoopOut::None,
                                                        Animation::InterpolationType::Linear);
                }
            }

            _players[targetId]->setHeadAngle(headAngle);
        } else if (targetId == _socket.ownId()) {
            _player->setHealth(health);
        }
    }
}

void ShooterClient::processNewClient(sf::Packet &packet) {
    sf::Uint16 targetId;

    packet >> targetId;

    if (_spawnPlayerCallBack != nullptr) {
        _spawnPlayerCallBack(targetId);
    }
}

void ShooterClient::processDisconnect(sf::Uint16 targetId) {
    if (targetId != _socket.ownId() && _players.count(targetId)) {
        _players.erase(targetId);
        _removePlayerCallBack(targetId);
    }
}


void ShooterClient::processCustomPacket(sf::Packet &packet) {
    sf::Uint16 buffId[2];
    double dbuff[10];
    std::string tmp, tmp2;

    ShooterMsgType type;
    packet >> type;

    switch (type) {
        case ShooterMsgType::Kill:
            packet >> buffId[0] >> buffId[1];
            _lastEvent = "";
            if (buffId[1] == _socket.ownId()) {
                _player->addKill();
                SoundController::loadAndPlay(SoundTag("kill"), ShooterConsts::KILL_SOUND);
                _lastEvent += _player->playerNickName();
            } else {
                _players[buffId[1]]->addKill();
                _lastEvent += _players[buffId[1]]->playerNickName();
            }
            _lastEvent += " ~> ";

            if (buffId[0] == _socket.ownId()) {
                _player->addDeath();

                auto camera = _player->attached(ObjectNameTag("Camera"));
                if (camera == nullptr) {
                    break;
                }

                _player->unattach(ObjectNameTag("Camera"));
                _player->translateToPoint(Vec3D{10000});
                camera->rotateLeft(-camera->angleLeftUpLookAt().x());
                camera->transform(Matrix4x4::Rotation(-_player->angle()));

                Timeline::addAnimation<ATranslateToPoint>(AnimationListTag("camera_anim"),
                                                          camera, Vec3D(0, 30, -100));
                Timeline::addAnimation<AWait>(AnimationListTag("camera_anim"), 0);
                Timeline::addAnimation<ARotateRelativePoint>(AnimationListTag("camera_anim"),
                                                             camera, Vec3D(0), Vec3D{0, Consts::PI, 0},
                                                             5, Animation::LoopOut::None,
                                                             Animation::InterpolationType::Linear);
                Timeline::addAnimation<AWait>(AnimationListTag("camera_anim"), 0);
                Timeline::addAnimation<AFunction>(AnimationListTag("camera_anim"), [this, camera]() {
                    // respawn
                    _player->translateToPoint(
                            Vec3D{50.0 * (-1 + 2.0 * (double) rand() / RAND_MAX), 30.0 * (double) rand() / RAND_MAX,
                                  50.0 * (-1 + 2.0 * (double) rand() / RAND_MAX)});
                    _player->reInitWeapons();
                    _player->setFullAbility();

                    camera->rotateToAngle(Vec3D(0));
                    camera->transform(Matrix4x4::Rotation(Vec3D(_player->angle())));
                    camera->rotateLeft(_player->headAngle());
                    camera->translateToPoint(_player->position() + Vec3D{0, 1.8, 0});
                    _player->attach(camera);

                }, 1, 0.1);


                SoundController::loadAndPlay(SoundTag("death"), ShooterConsts::DEATH_SOUND);
                _lastEvent += _player->playerNickName();
            } else {
                _players[buffId[0]]->addDeath();
                _lastEvent += _players[buffId[0]]->playerNickName();
            }
            break;
        case ShooterMsgType::FireTrace:
            packet >> dbuff[0] >> dbuff[1] >> dbuff[2] >> dbuff[3] >> dbuff[4] >> dbuff[5];

            if (_addFireTraceCallBack != nullptr) {
                _addFireTraceCallBack(Vec3D(dbuff[0], dbuff[1], dbuff[2]), Vec3D(dbuff[3], dbuff[4], dbuff[5]));
            }

            break;
        case ShooterMsgType::InitBonuses:
            while (packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2]) {
                if (_addBonusCallBack != nullptr) {
                    _addBonusCallBack(tmp, Vec3D(dbuff[0], dbuff[1], dbuff[2]));
                }
            }
            break;

        case ShooterMsgType::AddBonus:
            packet >> tmp >> dbuff[0] >> dbuff[1] >> dbuff[2];
            if (_addBonusCallBack != nullptr) {
                _addBonusCallBack(tmp, Vec3D(dbuff[0], dbuff[1], dbuff[2]));
            }

            break;
        case ShooterMsgType::RemoveBonus:
            packet >> tmp;
            if (_removeBonusCallBack != nullptr) {
                _removeBonusCallBack(ObjectNameTag(tmp));
            }
            break;
        case ShooterMsgType::ChangeWeapon:
            packet >> buffId[0] >> tmp;

            if (_changeEnemyWeaponCallBack != nullptr) {
                _changeEnemyWeaponCallBack(tmp, buffId[0]);
            }
            break;
        default:
            Log::log("ShooterClient::processCustomPacket: unknown message type " +
                     std::to_string(static_cast<int>(type)));
            return;
    }
}

void ShooterClient::processDisconnected() {
    for (auto it = _players.begin(); it != _players.end();) {
        processDisconnect(it++->first);
    }
}

void ShooterClient::damagePlayer(sf::Uint16 targetId, double damage) {
    sf::Packet packet;

    packet << MsgType::Custom << ShooterMsgType::Damage << targetId << damage;
    _socket.sendRely(packet, _socket.serverId());

    Log::log("ShooterClient: damagePlayer " + std::to_string(targetId) + " ( -" + std::to_string(damage) + "hp )");
}

void ShooterClient::addTrace(const Vec3D &from, const Vec3D &to) {
    sf::Packet packet;

    packet << MsgType::Custom << ShooterMsgType::FireTrace << from.x() << from.y() << from.z() << to.x() << to.y()
           << to.z();
    _socket.send(packet, _socket.serverId());
}

void ShooterClient::takeBonus(const std::string &bonusName) {
    sf::Packet packet;

    packet << MsgType::Custom << ShooterMsgType::RemoveBonus << bonusName;
    _socket.sendRely(packet, _socket.serverId());

    if (_removeBonusCallBack != nullptr) {
        _removeBonusCallBack(ObjectNameTag(bonusName));
    }
}

void ShooterClient::changeWeapon(const std::string &weaponName) {
    sf::Packet packet;

    packet << MsgType::Custom << ShooterMsgType::ChangeWeapon << weaponName;
    _socket.sendRely(packet, _socket.serverId());
}

void ShooterClient::addPlayer(sf::Uint16 id, std::shared_ptr<Player> player) {
    _players.insert({id, player});
}

void ShooterClient::setSpawnPlayerCallBack(std::function<void(sf::Uint16)> spawn) {
    _spawnPlayerCallBack = std::move(spawn);
}

void ShooterClient::setRemovePlayerCallBack(std::function<void(sf::Uint16)> remove) {
    _removePlayerCallBack = std::move(remove);
}

void ShooterClient::setAddFireTraceCallBack(std::function<void(const Vec3D &, const Vec3D &)> addTrace) {
    _addFireTraceCallBack = std::move(addTrace);
}

void ShooterClient::setAddBonusCallBack(std::function<void(const std::string &, const Vec3D &)> addBonus) {
    _addBonusCallBack = std::move(addBonus);
}

void ShooterClient::setRemoveBonusCallBack(std::function<void(const ObjectNameTag &)> removeBonus) {
    _removeBonusCallBack = std::move(removeBonus);
}

void
ShooterClient::setChangeEnemyWeaponCallBack(std::function<void(const std::string &, sf::Uint16)> changeEnemyWeapon) {
    _changeEnemyWeaponCallBack = std::move(changeEnemyWeapon);
}
