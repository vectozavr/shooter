//
// Created by Иван Ильин on 17.10.2021.
//

#include "SoundController.h"
#include "ResourceManager.h"

SoundController* SoundController::_instance = nullptr;

void SoundController::init() {
    _instance = new SoundController();
}

void SoundController::playSound(const SoundTag& soundTag, const std::string& filename) {
    if(!_instance)
        return;

    stopSound(soundTag);
    _instance->_sounds.emplace(soundTag, sf::Sound(*ResourceManager::loadSoundBuffer(filename)));
    _instance->_sounds[soundTag].play();
}

void SoundController::pauseSound(const SoundTag& soundTag) {
    if(!_instance)
        return;

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].pause();
    }
}

void SoundController::stopSound(const SoundTag& soundTag) {
    if(!_instance)
        return;

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].stop();
    }
    _instance->_sounds.erase(soundTag);
}

sf::Sound::Status SoundController::getStatus(const SoundTag& soundTag) {
    if(_instance == nullptr)
        return sf::Sound::Status::Stopped;

    if(_instance->_sounds.count(soundTag) > 0)
        return _instance->_sounds[soundTag].getStatus();
    else
        return sf::Sound::Status::Stopped;
}

void SoundController::free() {
    for(auto& [soundTag, sound] : _instance->_sounds) {
        sound.stop();
    }

    _instance->_sounds.clear();
    delete _instance;
    _instance = nullptr;
}
