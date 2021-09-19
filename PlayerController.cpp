//
// Created by Иван Ильин on 19.09.2021.
//

#include "PlayerController.h"

PlayerController::PlayerController(std::shared_ptr<Player> player,
                                   std::shared_ptr<World> world,
                                   std::shared_ptr<Keyboard> keyboard,
                                   std::shared_ptr<Mouse> mouse) : _player(player), _world(world), _keyboard(keyboard), _mouse(mouse) {
    _slowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/slow_mo.ogg"));
    _unSlowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/unslow_mo.ogg"));
}

void PlayerController::update() {
    // friction
    if(_player->inCollision())
        _player->setVelocity(_player->velocity()*(1.0 - Time::deltaTime() * 2));

    if(_isInSlowMo) {
        if(_player->ability() > 0)
            _player->setAbility(_player->ability() - Time::deltaTime());
        else {
            _player->setAbility(0);
            _isInSlowMo = false;
            _player->setVelocity(_player->velocity() * _slowMoCoefficient);
            _player->setAcceleration(_player->acceleration() * _slowMoCoefficient * _slowMoCoefficient);
            _slowMoSound.stop();
            _unSlowMoSound.play();
        }
    }

    double coeff = _isInSlowMo ? 1.0 / _slowMoCoefficient : 1.0;

    bool inRunning_old = _inRunning;
    _inRunning = (  Keyboard::isKeyPressed(sf::Keyboard::A) ||
                    Keyboard::isKeyPressed(sf::Keyboard::D) ||
                    Keyboard::isKeyPressed(sf::Keyboard::W) ||
                    Keyboard::isKeyPressed(sf::Keyboard::S));

    // in case when the camera is attached we make some animation during running
    if(_inRunning && !_player->camera()->isInAnim()) {
        _player->camera()->a_translate("hor_oscil", -_player->camera()->left()/12, 0.3/coeff, Animation::LoopOut::None, Animation::cos);
        _player->camera()->a_wait("hor_oscil", 0);
        _player->camera()->a_translate("hor_oscil", _player->camera()->left()/12, 0.3/coeff, Animation::LoopOut::None, Animation::cos);

        _player->camera()->a_translate("vert_oscil", -Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _player->camera()->a_wait("vert_oscil", 0);
        _player->camera()->a_translate("vert_oscil", Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _player->camera()->a_wait("vert_oscil", 0);
        _player->camera()->a_translate("vert_oscil", -Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);
        _player->camera()->a_wait("vert_oscil", 0);
        _player->camera()->a_translate("vert_oscil", Point4D{0, 1, 0}/24, 0.15/coeff, Animation::LoopOut::None, Animation::cos);

        _player->camera()->a_translateToPoint("init", _player->position() + Point4D{0, 1.8, 0}, 0.3/coeff, Animation::None, Animation::cos);

    } else if(inRunning_old && !_inRunning) {
        _player->camera()->a_stopAllAnimations();

        _player->camera()->a_translateToPoint("init", _player->position() + Point4D{0, 1.8, 0}, 0.15/coeff, Animation::None, Animation::cos);
    }

    auto rayToFloor = (*_world).rayCast(_player->position(), _player->position() + Point4D{0, -5, 0});

    // Left and right
    if (Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _player->translate(_player->camera()->left() * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _player->translate(-_player->camera()->left() * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }

    // Forward and backward
    if (Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _player->translate(_player->camera()->left().cross3D(Point4D{0, 1, 0}) * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _player->translate(-_player->camera()->left().cross3D(Point4D{0, 1, 0}) * Time::deltaTime() * _walkSpeed * coeff);

        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }

    if (_player->ability() > 0 && !_isInSlowMo && Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        // slow mo
        _isInSlowMo = true;
        _player->setVelocity(_player->velocity() / _slowMoCoefficient);
        _player->setAcceleration(_player->acceleration() / (_slowMoCoefficient * _slowMoCoefficient));
        _unSlowMoSound.stop();
        _slowMoSound.play();
    } else if (_isInSlowMo && !Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        _isInSlowMo = false;
        _player->setVelocity(_player->velocity() * _slowMoCoefficient);
        _player->setAcceleration(_player->acceleration() * _slowMoCoefficient * _slowMoCoefficient);
        _slowMoSound.stop();
        _unSlowMoSound.play();
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::Space) && _player->inCollision()) {
        _player->addVelocity(Point4D{0, std::abs(_player->collisionNormal().y()) * sqrt(2 * _g * _jumpHeight) * coeff, 0});
        _player->translate(Point4D{0, Time::deltaTime() * _walkSpeed * 2 * coeff, 0});
    }

    // Mouse movement
    Point4D disp = _mouse->getMouseDisplacement();

    _player->rotate(Point4D{0, -disp.x() / 1000.0, 0});
    _player->setVelocity(Matrix4x4::RotationY(-disp.x() / 1000.0) * _player->velocity());

    double rotationLeft = disp.y() / 1000.0;

    // You can only see in range [-90 : 90] grad
    if (_player->camera()->angleLeftUpLookAt().x() + rotationLeft > M_PI / 2)
        rotationLeft = M_PI / 2 - _player->camera()->angleLeftUpLookAt().x();
    if (_player->camera()->angleLeftUpLookAt().x() + rotationLeft < -M_PI / 2)
        rotationLeft = -M_PI / 2 - _player->camera()->angleLeftUpLookAt().x();

    _player->camera()->rotateLeft(rotationLeft);
    _player->rotateWeaponsRelativePoint(_player->position() + Point4D{0, 1.8, 0}, _player->camera()->left(), rotationLeft);

    if (_keyboard->isKeyTapped(sf::Keyboard::Right) || _keyboard->isKeyTapped(sf::Keyboard::E)) {
        _player->nextWeapon();
    }

    if (_keyboard->isKeyTapped(sf::Keyboard::Left) || _keyboard->isKeyTapped(sf::Keyboard::Q)) {
        _player->previousWeapon();
    }

    if (Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        _player->fire();
    }

    if(Keyboard::isKeyPressed(sf::Keyboard::R)) {
        _player->reload();
    }

    if (_inRunning && _player->inCollision() && _walkSound.getStatus() != sf::Sound::Status::Playing) {
        if ((_player->position() - rayToFloor.first).abs() < 2) {
            int soundNum = round((double) rand() / RAND_MAX * 5) + 1;
            _walkSound.setBuffer(*ResourceManager::loadSoundBuffer("../sound/stonestep" + std::to_string(soundNum) + ".ogg"));
            _walkSound.play();
        }
    }
}
