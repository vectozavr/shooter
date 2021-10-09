//
// Created by Иван Ильин on 19.09.2021.
//

#include "PlayerController.h"
#include "engine/utils/Log.h"
#include "engine/animation/AColor.h"
#include "engine/animation/AFunction.h"
#include "engine/animation/AWait.h"
#include "engine/animation/ATranslate.h"
#include "engine/animation/ATranslateToPoint.h"
#include "engine/animation/Timeline.h"

PlayerController::PlayerController(std::shared_ptr<Player> player,
                                   std::shared_ptr<Keyboard> keyboard,
                                   std::shared_ptr<Mouse> mouse) : _player(player), _keyboard(keyboard), _mouse(mouse) {
    _slowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/slow_mo.ogg"));
    _unSlowMoSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/unslow_mo.ogg"));
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

    std::shared_ptr<Object> camera = _player->attached("camera");
    if(_inRunning) {
        if (!Timeline::isInAnimList("camera_hor_oscil")) {
            Timeline::animate("camera_hor_oscil", new ATranslate(camera, -camera->left() / 6, 0.3,Animation::LoopOut::None, Animation::InterpolationType::cos));
            Timeline::animate("camera_hor_oscil", new AWait(0));
            Timeline::animate("camera_hor_oscil", new ATranslate(camera, camera->left() / 6, 0.3, Animation::LoopOut::None, Animation::InterpolationType::cos));

            Timeline::animate("camera_vert_oscil", new ATranslate(camera, -Point4D{0, 1, 0} / 12, 0.15, Animation::LoopOut::None, Animation::InterpolationType::cos));
            Timeline::animate("camera_vert_oscil", new AWait(0));
            Timeline::animate("camera_vert_oscil", new ATranslate(camera, Point4D{0, 1, 0} / 12, 0.15, Animation::LoopOut::None, Animation::InterpolationType::cos));
            Timeline::animate("camera_vert_oscil", new AWait(0));
            Timeline::animate("camera_vert_oscil", new ATranslate(camera, -Point4D{0, 1, 0} / 12, 0.15, Animation::LoopOut::None, Animation::InterpolationType::cos));
            Timeline::animate("camera_vert_oscil", new AWait(0));
            Timeline::animate("camera_vert_oscil", new ATranslate(camera, Point4D{0, 1, 0} / 12, 0.15, Animation::LoopOut::None, Animation::InterpolationType::cos));

            Timeline::animate("camera_init", new ATranslateToPoint( camera, _player->position() + Point4D{0, 1.8, 0}, 0.3, Animation::LoopOut::None, Animation::InterpolationType::cos));
        }
    } else if(inRunning_old && !_inRunning) {
        Timeline::deleteAnimationList("camera_hor_oscil");
        Timeline::deleteAnimationList("camera_vert_oscil");
        Timeline::deleteAnimationList("camera_init");
        Timeline::animate("camera_init", new ATranslateToPoint( camera, _player->position() + Point4D{0, 1.8, 0}, 0.15, Animation::LoopOut::None, Animation::InterpolationType::cos));
    }

    // Left and right

    if (Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _player->translate(_player->left() * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _player->translate(-_player->left() * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }

    // Forward and backward
    if (Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _player->translate(_player->lookAt() * Time::deltaTime() * _walkSpeed * coeff);
        if(_player->inCollision())
            _player->setVelocity(Point4D{0,0,0});
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _player->translate(-_player->lookAt() * Time::deltaTime() * _walkSpeed * coeff);

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

        // if we just want to jump, we have to add particular speed
        if (!_isSliding)
            _player->addVelocity(Point4D{ 0, std::abs(_player->collisionNormal().y()) * sqrt(2 * -_player->acceleration().y() * _jumpHeight) * coeff, 0 });
        // if we want to slide, we have to add speed * 60/fps to make it independent on frame rate
        else
            _player->addVelocity(Point4D{ 0, std::abs(_player->collisionNormal().y()) * sqrt(2 * -_player->acceleration().y() * _jumpHeight) * coeff * 60.0 / Time::fps(), 0 });

        _player->translate(Point4D{ 0, Time::deltaTime() * _walkSpeed * 2 * coeff * 60.0 / Time::fps(), 0 });
        _isSliding = true;
    } else {
        _isSliding = false;
    }

    // Mouse movement
    Point4D displacement = _mouse->getMouseDisplacement();

    _player->rotate(Point4D{0, -displacement.x() / 1000.0, 0});
    _player->setVelocity(Matrix4x4::RotationY(-displacement.x() / 1000.0) * _player->velocity());

    double rotationLeft = displacement.y() / 1000.0;

    // You can only see in range [-90 : 90] grad
    if (_player->headAngle() + rotationLeft > Consts::PI / 2)
        rotationLeft = Consts::PI / 2 - _player->headAngle();
    if (_player->headAngle() + rotationLeft < -Consts::PI / 2)
        rotationLeft = -Consts::PI / 2 - _player->headAngle();

    _player->setHeadAngle(_player->headAngle() + rotationLeft);
    _player->rotateWeaponsRelativePoint(_player->position() + Point4D{0, 1.8, 0}, _player->left(), rotationLeft);

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

    if ((_inRunning || _player->velocity().sqrAbs() > 3) && _player->inCollision() && _walkSound.getStatus() != sf::Sound::Status::Playing) {
        int soundNum = (int)((double) rand() / RAND_MAX * 5) + 1;
        _walkSound.setBuffer(*ResourceManager::loadSoundBuffer("sound/stonestep" + std::to_string(soundNum) + ".ogg"));
        _walkSound.play();
    }
}
