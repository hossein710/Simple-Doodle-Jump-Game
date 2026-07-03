#include "PlatformManager.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BreakablePlatform.hpp"
#include "ResourceManager.hpp"
#include "Constants.hpp"
#include <ctime>
#include <algorithm>

PlatformManager::PlatformManager(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      rng(static_cast<unsigned int>(std::time(nullptr))) {
    spawnInitial();
}

void PlatformManager::reset() {
    platforms.clear();
    previous_broken = false;
    spawnInitial();
}

std::unique_ptr<Platform> PlatformManager::createRandomPlatform(float y, bool forceSolid, bool& outIsBreakable) {
    std::uniform_real_distribution<float> xDist(
        Constants::PLATFORM_WIDTH / 2.f,
        static_cast<float>(windowWidth) - Constants::PLATFORM_WIDTH / 2.f);
    std::uniform_real_distribution<float> chance(0.f, 1.f);

    float x = xDist(rng);
    float roll = chance(rng);

    auto& textures = ResourceManager<sf::Texture>::instance();

    std::unique_ptr<Platform> platform;
    outIsBreakable = false;

    // forceSolid overrides the roll: if allowing a breakable here would push
    // the distance since the last reliable platform beyond what's reachable,
    // we must produce a non-breakable platform regardless of chance.
    if (!forceSolid && !previous_broken && roll < Constants::BREAKABLE_CHANCE) {
        platform = std::make_unique<BreakablePlatform>(
            textures.get("assets/broken_platform.png"), sf::Vector2f(x, y));
            previous_broken = true;
            outIsBreakable = true;
    } else if (roll < Constants::BREAKABLE_CHANCE + Constants::MOVING_CHANCE) {
        platform = std::make_unique<MovingPlatform>(
            textures.get("assets/moving_platform.png"), sf::Vector2f(x, y),
            windowWidth, Constants::MOVING_PLATFORM_SPEED);
            previous_broken = false;
    } else {
        platform = std::make_unique<NormalPlatform>(
            textures.get("assets/normal_platform.png"), sf::Vector2f(x, y));

        // Springs only ever appear on normal platforms, and only sometimes.
        if (chance(rng) < Constants::SPRING_CHANCE) {
            platform->attachSpring(textures.get("assets/spring_sprite.png"));
        }

        previous_broken = false;
    }

    return platform;
}

void PlatformManager::spawnInitial() {
    // One guaranteed safe platform right under the player's starting spot.
    auto& textures = ResourceManager<sf::Texture>::instance();
    platforms.push_back(std::make_unique<NormalPlatform>(
        textures.get("assets/normal_platform.png"),
        sf::Vector2f(windowWidth / 2.f, windowHeight - 60.f)));

    highestY = windowHeight - 60.f;
    lastSolidY = highestY;

    // Fill the rest of the initial screen with reachable platforms.
    while (highestY > 0.f) {
        std::uniform_real_distribution<float> gapDist(Constants::MIN_VERTICAL_GAP,
                                                      Constants::MAX_VERTICAL_GAP);

        float nextY = highestY - gapDist(rng);
        bool forceSolid = false;

        // If this next height would exceed the maximum reachable jump from our last solid anchor,
        // clamp it to the maximum allowable gap and force it to be solid.
        if ((lastSolidY - nextY) > Constants::MAX_VERTICAL_GAP) {
            nextY = lastSolidY - Constants::MAX_VERTICAL_GAP;
            forceSolid = true;
        }

        highestY = nextY;

        bool isBreakable = false;
        platforms.push_back(createRandomPlatform(highestY, forceSolid, isBreakable));

        if (!isBreakable) {
            lastSolidY = highestY;
        }
    }
}

void PlatformManager::maintain() {
    // Keep generating upward while the topmost platform is still on/above screen.
    while (highestY > -Constants::MAX_VERTICAL_GAP) {
        std::uniform_real_distribution<float> gapDist(Constants::MIN_VERTICAL_GAP,
                                                      Constants::MAX_VERTICAL_GAP);

        float nextY = highestY - gapDist(rng);
        bool forceSolid = false;

        // If this next height would exceed the maximum reachable jump from our last solid anchor,
        // clamp it to the maximum allowable gap and force it to be solid.
        if ((lastSolidY - nextY) > Constants::MAX_VERTICAL_GAP) {
            nextY = lastSolidY - Constants::MAX_VERTICAL_GAP;
            forceSolid = true;
        }

        highestY = nextY;

        bool isBreakable = false;
        platforms.push_back(createRandomPlatform(highestY, forceSolid, isBreakable));

        if (!isBreakable) {
            lastSolidY = highestY;
        }
    }

    // Remove platforms that scrolled off the bottom edge of the screen
    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(),
                        [this](const std::unique_ptr<Platform>& p) {
                            return p->getPosition().y > static_cast<float>(windowHeight) + 60.f;
                        }),
        platforms.end());
}

void PlatformManager::update(float dt) {
    for (auto& platform : platforms) {
        platform->update(dt);
    }
}

void PlatformManager::scroll(float dy) {
    for (auto& platform : platforms) {
        platform->move(0.f, dy);
    }
    highestY += dy;
    lastSolidY += dy;
    maintain();
}

void PlatformManager::render(sf::RenderWindow& window) {
    for (auto& platform : platforms) {
        platform->render(window);
    }
}
