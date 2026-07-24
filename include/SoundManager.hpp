#pragma once
#include <SFML/Audio.hpp>
#include <string>

class SoundManager {
public:
    static SoundManager& instance();

    // Background Music (Streaming via sf::Music)
    bool loadBGM(const std::string& filepath);
    void playBGM();
    void stopBGM();

    // Sound Effects using ResourceManager<sf::SoundBuffer>
    void preloadSound(const std::string& filepath);
    void playSound(const std::string& filepath);

    void setVolume(float volume); // 0.0 to 100.0
    float getVolume() const { return currentVolume; }

private:
    SoundManager() = default;

    sf::Music bgMusic;
    sf::Sound soundPlayer;
    float currentVolume = 80.f;
};