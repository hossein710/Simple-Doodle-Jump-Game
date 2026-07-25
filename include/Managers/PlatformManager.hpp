#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Platform.hpp"
#include "Monster.hpp"
#include "Hole.hpp"
#include "Constants.hpp"

class PlatformManager {
public:
    PlatformManager(unsigned int windowWidth, unsigned int windowHeight);

    void reset(Difficulty difficulty);
    void update(float dt);
    void scroll(float dy); 
    void render(sf::RenderWindow& window);

    void maintain(Difficulty difficulty, float playerClimbingScore);

    // Shared collision check function to avoid overlapping objects
    bool isPositionValid(const sf::FloatRect& candidateBounds) const;

    std::vector<std::unique_ptr<Platform>>& getPlatforms() { return platforms; }
    std::vector<std::unique_ptr<Monster>>& getMonsters() { return monsters; }
    std::vector<std::unique_ptr<Hole>>& getHoles() { return holes; }

private:
    void spawnInitial(Difficulty difficulty = Difficulty::Easy);
    std::unique_ptr<Platform> createRandomPlatform(float y, bool forceSolid, bool& outIsBreakable, Difficulty difficulty);
    void trySpawnMonsterOrHole(float y, Difficulty difficulty, float playerClimbingScore);

    unsigned int windowWidth;
    unsigned int windowHeight;

    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::unique_ptr<Hole>> holes;

    float highestY = 0.f;   
    float lastSolidY = 0.f; 

    std::mt19937 rng;
    bool previous_broken = false;
};
