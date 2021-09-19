//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "utils/Log.h"

void Player::update() {

    // friction
    if(inCollision())
        _velocity = _velocity - _velocity * Time::deltaTime() * 2;

    if(_isInSlowMo) {
        if(_ability > 0)
            _ability -= Time::deltaTime();
        else {
            _ability = 0;
            _isInSlowMo = false;
            setVelocity(velocity() * _slowMoCoefficient);
            setAcceleration(_acceleration * _slowMoCoefficient * _slowMoCoefficient);
            _slowMoSound.stop();
            _unSlowMoSound.play();
        }
    }

    double coeff = _isInSlowMo ? 1.0 / _slowMoCoefficient : 1.0;

    bool inRunning_old = _inRunning;
    _inRunning = _screen != nullptr && (Screen::isKeyPressed(sf::Keyboard::A) || Screen::isKeyPressed(sf::Keyboard::D) || Screen::isKeyPressed(sf::Keyboard::W) || Screen::isKeyPressed(sf::Keyboard::S));

    // in case when the camera is attached we make some animation during running
    if(_camera != nullptr && _inRunning && !_camera->isInAnim()) {
        _camera->a_translate("hor_oscil", -_camera->left()/12, 0.3/coeff, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("hor_oscil", 0);
        _camera->a_translate("hor_oscil", _camera->left()/12, 0.3/coeff, Animation::LoopOut::None, Animation::cos);

        _camera->a_translate("vert_oscil", -Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", -Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _camera->a_wait("vert_oscil", 0);
        _camera->a_translate("vert_oscil", Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);

        _camera->a_translateToPoint("init", position() + Point4D{0, 1.8, 0}, 0.3/coeff, Animation::None, Animation::cos);

    } else if(_camera != nullptr && inRunning_old && !_inRunning) {
        _camera->a_stopAllAnimations();

        _camera->a_translateToPoint("init", position() + Point4D{0, 1.8, 0}, 0.15/coeff, Animation::None, Animation::cos);
    }

    auto rayToFloor = (*_world).rayCast(position(), position() + Point4D{0, -5, 0});

    if(_world != nullptr && _screen != nullptr && _camera != nullptr) {
        // Left and right
        if (Screen::isKeyPressed(sf::Keyboard::A)) {
            translate(_camera->left() * Time::deltaTime() * _walkSpeed * coeff);
            if(inCollision())
                setVelocity(Point4D{0,0,0});
        }
        if (Screen::isKeyPressed(sf::Keyboard::D)) {
            translate(-_camera->left() * Time::deltaTime() * _walkSpeed * coeff);
            if(inCollision())
                setVelocity(Point4D{0,0,0});
        }

        // Forward and backward
        if (Screen::isKeyPressed(sf::Keyboard::W)) {
            translate(_camera->left().cross3D(Point4D{0, 1, 0}) * Time::deltaTime() * _walkSpeed * coeff);
            if(inCollision())
                setVelocity(Point4D{0,0,0});
        }

        if (Screen::isKeyPressed(sf::Keyboard::S)) {
            translate(-_camera->left().cross3D(Point4D{0, 1, 0}) * Time::deltaTime() * _walkSpeed * coeff);

            if(inCollision())
                setVelocity(Point4D{0,0,0});
        }

        if (_ability > 0 && !_isInSlowMo && Screen::isKeyPressed(sf::Keyboard::LShift)) {
            // slow mo
            _isInSlowMo = true;
            setVelocity(velocity() / _slowMoCoefficient);
            setAcceleration(_acceleration / (_slowMoCoefficient * _slowMoCoefficient));
            _unSlowMoSound.stop();
            _slowMoSound.play();
        } else if (_isInSlowMo && !Screen::isKeyPressed(sf::Keyboard::LShift)) {
            _isInSlowMo = false;
            setVelocity(velocity() * _slowMoCoefficient);
            setAcceleration(_acceleration * _slowMoCoefficient * _slowMoCoefficient);
            _slowMoSound.stop();
            _unSlowMoSound.play();
        }

        if (Screen::isKeyPressed(sf::Keyboard::Space) && inCollision()) {
            addVelocity(Point4D{0, std::abs(_collisionNormal.y()) * sqrt(2 * _g * _jumpHeight) * coeff, 0});
            translate(Point4D{0, Time::deltaTime() * _walkSpeed * 2 * coeff, 0});
        }

        // Mouse movement
        Point4D disp = _screen->getMouseDisplacement();

        rotate(Point4D{0, -disp.x() / 1000.0, 0});
        _velocity = Matrix4x4::RotationY(-disp.x() / 1000.0) * _velocity;

        double rotationLeft = disp.y() / 1000.0;

        // You can only see in range [-90 : 90] grad
        if (_camera->angleLeftUpLookAt().x() + rotationLeft > M_PI / 2)
            rotationLeft = M_PI / 2 - _camera->angleLeftUpLookAt().x();
        if (_camera->angleLeftUpLookAt().x() + rotationLeft < -M_PI / 2)
            rotationLeft = -M_PI / 2 - _camera->angleLeftUpLookAt().x();

        _camera->rotateLeft(rotationLeft);
        rotateWeaponsRelativePoint(position() + Point4D{0, 1.8, 0}, _camera->left(), rotationLeft);

        if (_screen->isKeyTapped(sf::Keyboard::Right) || _screen->isKeyTapped(sf::Keyboard::E)) {
            if(_weapons.size() > 1) {
                // change '_selectedWeapon'
                _weapons[_selectedWeapon]->removeFromWorld(_world);
                _selectedWeapon = (_selectedWeapon + 1) % _weapons.size();
                _weapons[_selectedWeapon]->addToWorld(_world);
                Log::log("selected _selectedWeapon " + std::to_string(_selectedWeapon));
                _changeWeaponSound.play();
            }
        }

        if (_screen->isKeyTapped(sf::Keyboard::Left) || _screen->isKeyTapped(sf::Keyboard::Q)) {
            if(_weapons.size() > 1) {
                // change '_selectedWeapon'
                _weapons[_selectedWeapon]->removeFromWorld(_world);
                if (_selectedWeapon > 0)
                    _selectedWeapon = (_selectedWeapon - 1) % _weapons.size();
                else
                    _selectedWeapon = _weapons.size() - 1;
                _weapons[_selectedWeapon]->addToWorld(_world);
                Log::log("selected _selectedWeapon " + std::to_string(_selectedWeapon));
                _changeWeaponSound.play();
            }
        }

        if (_screen->isButtonPressed(sf::Mouse::Button::Left)) {
            auto damagedPlayers = _weapons[_selectedWeapon]->fire(_world, _camera);
            for(auto& damagedPlayer : damagedPlayers) {
                sf::Uint16 targetId = std::stoi(damagedPlayer.first.substr(7));
                _damagePlayerCallBack(targetId, damagedPlayer.second);
            }
        }

        if(Screen::isKeyPressed(sf::Keyboard::R)) {
            _weapons[_selectedWeapon]->reload();
        }

        if (_inRunning && inCollision() && _walkSound.getStatus() != sf::Sound::Status::Playing) {
            if ((position() - rayToFloor.first).abs() < 2) {
                int soundNum = round((double) rand() / RAND_MAX * 5) + 1;
                _walkSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/stonestep" + std::to_string(soundNum) + ".ogg"));
                _walkSound.play();
            }
        }
    }
}

void Player::rotateWeaponsRelativePoint(const Point4D& point4D, const Point4D& v, double val) {
    for(auto& weapon : _weapons)
        weapon->rotateRelativePoint(point4D, v, val);
}

void Player::drawStats() {

    if(_screen == nullptr)
        return;

    // health bar
    int xPos = 10;
    int yPos = _screen->height() - 10 - 10;

    int width = _screen->width()/2 - 20;
    int height = 10;

    sf::ConvexShape polygon1;
    polygon1.setPointCount(4);
    sf::ConvexShape polygon2;
    polygon2.setPointCount(4);

    polygon1.setPoint(0, sf::Vector2f((float)xPos, (float)yPos));
    polygon1.setPoint(1, sf::Vector2f((float)(xPos + width), (float)yPos));
    polygon1.setPoint(2, sf::Vector2f((float)(xPos + width), (float)(yPos + height)));
    polygon1.setPoint(3, sf::Vector2f((float)xPos, (float)(yPos + height)));

    polygon2.setPoint(0, sf::Vector2f((float)xPos, (float)yPos));
    polygon2.setPoint(1, sf::Vector2f((float)xPos + width * _health / _healthMax, (float)yPos));
    polygon2.setPoint(2, sf::Vector2f((float)xPos + width * _health / _healthMax, (float)(yPos + height)));
    polygon2.setPoint(3, sf::Vector2f((float)xPos, (float)(yPos + height)));

    polygon1.setFillColor({ 255, 174, 174, 100 });
    polygon2.setFillColor({ static_cast<sf::Uint8>((_healthMax - _health)/_healthMax * 255), static_cast<sf::Uint8>(_health * 255 / _healthMax), 0, 100 });

    polygon1.setOutlineThickness(3);
    _screen->window.draw(polygon2);

    // ability bar
    sf::ConvexShape polygon3;
    polygon3.setPointCount(4);
    polygon3.setPoint(0, sf::Vector2f((float)xPos, (float)yPos - 15));
    polygon3.setPoint(1, sf::Vector2f((float)xPos + width * _ability / _abilityMax, (float)yPos - 15));
    polygon3.setPoint(2, sf::Vector2f((float)xPos + width * _ability / _abilityMax, (float)(yPos - 15 + height)));
    polygon3.setPoint(3, sf::Vector2f((float)xPos, (float)(yPos - 15 + height)));
    polygon3.setFillColor({ 255, 168, 168, 100 });
    _screen->window.draw(polygon3);

    // ammo
    sf::Text text_health;

    text_health.setFont(*ResourceManager::loadFont("../engine/fonts/Roboto-Medium.ttf"));

    // text health
    text_health.setCharacterSize(20);
    text_health.setFillColor(sf::Color::White);
    text_health.setStyle(sf::Text::Italic);
    text_health.setPosition(0, 0);

    text_health.setString(std::to_string((int)_health));

    // text ammo
    sf::Text text_ammo1(text_health);
    sf::Text text_ammo2(text_health);

    int ammo1Size = 100;
    int ammo2Size = 50;

    auto balance = _weapons[_selectedWeapon]->balance();

    text_ammo1.setCharacterSize(ammo1Size);
    text_ammo1.setString(std::to_string((int)balance.first));
    text_ammo1.setPosition(150, _screen->height() - 50 - ammo1Size);
    text_ammo1.setFillColor(sf::Color(0, 0, 0, 100));
    _screen->window.draw(text_ammo1);

    text_ammo2.setCharacterSize(ammo2Size);
    text_ammo2.setString(std::to_string((int)balance.second));
    text_ammo2.setPosition(50, _screen->height() - 50 - ammo2Size);
    text_ammo2.setFillColor(sf::Color(0, 0, 0, 70));
    _screen->window.draw(text_ammo2);

    // text _killSound/_deathSound stats
    sf::Text text_kills(text_health);
    text_kills.setStyle(sf::Text::Bold);
    text_kills.setString("KILLS: " + std::to_string((int)_kills) + " | " + "DEATHS: " + std::to_string((int)_deaths));
    text_kills.setFillColor(sf::Color(0, 0, 0, 100));
    text_kills.setCharacterSize(ammo2Size/2);
    text_kills.setPosition(10, 10);

    _screen->window.draw(text_kills);

    sf::Text text_deaths(text_health);

    text_deaths.setString(std::to_string((int)_deaths));
    text_deaths.setFillColor(sf::Color(100, 0, 0, 100));
    text_deaths.setCharacterSize(ammo2Size);
    text_deaths.setPosition(10, ammo2Size + 10);
}

void Player::playDeath() {
    _deathSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/classic_hurt.ogg"));
    _deathSound.play();
}

void Player::playKill() {
    _killSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/kill.ogg"));
    _killSound.play();
}

void Player::collisionWithObject(const std::string &objName, std::shared_ptr<RigidBody> obj) {
    if(objName.find("Bonus_gun") != std::string::npos)
        addWeapon(std::make_shared<Gun>());

    if(objName.find("Bonus_shotgun") != std::string::npos)
        addWeapon(std::make_shared<Shotgun>());

    if(objName.find("Bonus_ak47") != std::string::npos)
        addWeapon(std::make_shared<Ak47>());

    if(objName.find("Bonus_gold_ak47") != std::string::npos)
        addWeapon(std::make_shared<Gold_Ak47>());

    if(objName.find("Bonus_rifle") != std::string::npos)
        addWeapon(std::make_shared<Rifle>());

    if(objName.find("Bonus_hill") != std::string::npos)
        setFullHealth();

    if(objName.find("Bonus_ability") != std::string::npos)
        setFullAbility();

    if(objName.find("Bonus") != std::string::npos) {
        _world->removeBody(objName);
        _takeBonusCallBack(objName);
    }
}

void Player::addWeapon(std::shared_ptr<Weapon> weapon) {
    _changeWeaponSound.play();

    if(!_weapons.empty()) {
        for(auto& w : _weapons) {
            if (w->name() == weapon->name()) {
                w->addAmmo(w->initialPack());
                return;
            }
        }
    }

    _weapons.push_back(weapon);
    _weapons.back()->attachToPlayer(*this);

    _weapons.back()->translate(position());
    _weapons.back()->rotateRelativePoint(position() + Point4D{0, 1.8, 0}, Point4D{0, 1, 0}, _angle.y());
    _weapons.back()->rotateRelativePoint(position() + Point4D{0, 1.8, 0}, _camera->left(), _camera->angleLeftUpLookAt().x());

    _weapons.back()->setAddTraceCallBack(_addTraceCallBack);
    _changeWeaponSound.play();
}

void Player::initWeapons() {

    if(!_weapons.empty()) {
        _weapons[_selectedWeapon]->removeFromWorld(_world);
        _weapons.clear();
    }

    _selectedWeapon = 0;
    addWeapon(std::make_shared<Gun>());

    _weapons[_selectedWeapon]->addToWorld(_world);
}
