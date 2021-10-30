//
// Created by Иван Ильин on 17.10.2021.
//

#include "SoundController.h"
#include "ResourceManager.h"
#include "utils/Log.h"

SoundController* SoundController::_instance = nullptr;
bool SoundController::_validInstance = false;


void SoundController::init() {
    _instance = new SoundController();
    _validInstance = true;

    Log::log("SoundController::init(): sound controller was initialized");
}

void SoundController::playSound(const SoundTag& soundTag, const std::string& filename) {
    if(!_validInstance) {
        return;
    }

    stopSound(soundTag);
    _instance->_sounds.emplace(soundTag, sf::Sound(*ResourceManager::loadSoundBuffer(filename)));
    _instance->_sounds[soundTag].play();

    Log::log("SoundController::playSound(): play sound '" + soundTag.str() + "'");
}

void SoundController::pauseSound(const SoundTag& soundTag) {
    if(!_validInstance) {
        return;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].pause();
    }

    Log::log("SoundController::pauseSound(): sound '" + soundTag.str() + "' was paused");
}

void SoundController::stopSound(const SoundTag& soundTag) {
    if(!_validInstance) {
        return;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        _instance->_sounds[soundTag].stop();
    }
    _instance->_sounds.erase(soundTag);

    Log::log("SoundController::stopSound(): sound '" + soundTag.str() + "' was stopped");
}

sf::Sound::Status SoundController::getStatus(const SoundTag& soundTag) {
    if(!_validInstance) {
        return sf::Sound::Status::Stopped;
    }

    if(_instance->_sounds.count(soundTag) > 0) {
        return _instance->_sounds[soundTag].getStatus();
    } else {
        _instance->_sounds.erase(soundTag);
        return sf::Sound::Status::Stopped;
    }
}

void SoundController::free() {
    if(_validInstance) {
        for(auto& [soundTag, sound] : _instance->_sounds) {
            _instance->_sounds[soundTag].stop();
            Log::log("SoundController::stopSound(): sound '" + soundTag.str() + "' was stopped");
        }
        _instance->_sounds.clear();
    }

    _validInstance = false;
    delete _instance;

    Log::log("SoundController::free(): pointer to 'SoundController' was freed");
}
