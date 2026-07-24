#include "SoundManager.hpp"
#include "ResourceManager.hpp"

SoundManager& SoundManager::instance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadBGM(const std::string& filepath) {
    if (bgMusic.openFromFile(filepath)) {
        bgMusic.setLoop(true);
        bgMusic.setVolume(currentVolume);
        return true;
    }
    return false;
}

void SoundManager::playBGM() {
    if (bgMusic.getStatus() != sf::Music::Playing) {
        bgMusic.play();
    }
}

void SoundManager::stopBGM() {
    if (bgMusic.getStatus() == sf::Music::Playing) {
        bgMusic.stop();
    }
}

void SoundManager::preloadSound(const std::string& filepath) {
    // Fetches and caches the buffer inside ResourceManager
    ResourceManager<sf::SoundBuffer>::instance().get(filepath);
}

void SoundManager::playSound(const std::string& filepath) {
    // Get reference to the cached sound buffer
    sf::SoundBuffer& buffer = ResourceManager<sf::SoundBuffer>::instance().get(filepath);
    
    soundPlayer.setBuffer(buffer);
    soundPlayer.setVolume(currentVolume);
    soundPlayer.play();
}

void SoundManager::setVolume(float volume) {
    currentVolume = volume;
    bgMusic.setVolume(currentVolume);
    soundPlayer.setVolume(currentVolume);
}