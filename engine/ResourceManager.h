//
// Created by Neirokan on 09.05.2020
//

#ifndef ENGINE_RESOURCEMANAGER_H
#define ENGINE_RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

namespace ResourceManager
{
    // Unloads all currently loaded textures.
    void unloadTextures();
    void unloadSoundBuffers();
    void unloadFonts();
    void unloadShaders();

    void unloadAllResources();

    // Try to load texture from file.
    // If success returns pointer to texture.
    // Otherwise returns nullptr.
    std::shared_ptr<sf::Texture> loadTexture(const std::string& filename);
    std::shared_ptr<sf::Font> loadFont(const std::string& filename);
    std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& filename);
    std::shared_ptr<sf::Shader> loadShader(const std::string& filename, sf::Shader::Type type);
};


#endif //PSEUDO3DENGINE_RESOURCEMANAGER_H
