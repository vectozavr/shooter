//
// Created by Neirokan on 09.05.2020
//

#include "ResourceManager.h"
#include <map>
#include <memory>

namespace ResourceManager
{
	namespace
	{
		std::map<std::string, std::shared_ptr<sf::Texture>> _textures;
        std::map<std::string, std::shared_ptr<sf::Font>> _fonts;
		std::map<std::string, std::shared_ptr<sf::SoundBuffer>> _soundBuffers;
	}

	void unloadTextures()
	{
		for (auto & _texture : _textures)
			_texture.second.reset();
		_textures.clear();
	}

	void unloadSoundBuffers()
	{
		for (auto & _soundBuffer : _soundBuffers)
			_soundBuffer.second.reset();
		_soundBuffers.clear();
	}

    void unloadFonts() {
        for (auto & _font : _fonts)
            _font.second.reset();
        _fonts.clear();
	}

	void unloadAllResources()
	{
		unloadTextures();
		unloadSoundBuffers();
        unloadFonts();
	}

	std::shared_ptr<sf::Texture> loadTexture(const std::string& filename)
	{
		// If texture is already loaded - return pointer to it
		auto it = _textures.find(filename);
		if (it != _textures.end())
			return it->second;

		// Otherwise - try to load it. If failure - return zero
		std::shared_ptr<sf::Texture> texture(new sf::Texture);
		if (!texture->loadFromFile(filename))
			return nullptr;

		// If success - remember and return texture pointer
		texture->setRepeated(true);
		_textures.emplace(filename, texture);

		return texture;
	}

    std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& filename)
	{
		// If sound buffer is already loaded - return pointer to it
		auto it = _soundBuffers.find(filename);
		if (it != _soundBuffers.end())
			return it->second;

		// Otherwise - try to load it. If failure - return zero
		std::shared_ptr<sf::SoundBuffer> soundBuffer(new sf::SoundBuffer);
		if (!soundBuffer->loadFromFile(filename))
			return nullptr;

		// If success - remember and return texture pointer
		_soundBuffers.emplace(filename, soundBuffer);

		return soundBuffer;
	}

    std::shared_ptr<sf::Font> loadFont(const std::string& filename) {
        // If font is already loaded - return pointer to it
        auto it = _fonts.find(filename);
        if (it != _fonts.end())
            return it->second;

        // Otherwise - try to load it. If failure - return zero
        std::shared_ptr<sf::Font> font(new sf::Font);
        if (!font->loadFromFile(filename))
            return nullptr;

        // If success - remember and return texture pointer
        _fonts.emplace(filename, font);

        return font;
	}
}
