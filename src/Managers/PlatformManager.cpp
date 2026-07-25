#include "PlatformManager.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BreakablePlatform.hpp"
#include "ResourceManager.hpp"
#include <ctime>
#include <algorithm>

PlatformManager::PlatformManager(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      rng(static_cast<unsigned int>(std::time(nullptr))) {
    spawnInitial(Difficulty::Easy);
}

void PlatformManager::reset(Difficulty difficulty) {
    platforms.clear();
    monsters.clear();
    holes.clear();
    previous_broken = false;
    spawnInitial(difficulty);
}

bool PlatformManager::isPositionValid(const sf::FloatRect& candidateBounds) const {
    for (const auto& p : platforms) {
        if (p->getBounds().intersects(candidateBounds)) return false;
        if (p->isMovingPlatform()){
            sf::FloatRect candidateBounds2(0.f, candidateBounds.top, Constants::WINDOW_WIDTH, candidateBounds.height);
            if (p->getBounds().intersects(candidateBounds2)) return false;
        }
    }
    for (const auto& m : monsters) {
        if (m->getBounds().intersects(candidateBounds)) return false;
    }
    for (const auto& h : holes) {
        if (h->getBounds().intersects(candidateBounds)) return false;
    }
    return true;
}

std::unique_ptr<Platform> PlatformManager::createRandomPlatform(float y, bool forceSolid, bool& outIsBreakable, Difficulty difficulty) {
    std::uniform_real_distribution<float> xDist(
        Constants::PLATFORM_WIDTH / 2.f,
        static_cast<float>(windowWidth) - Constants::PLATFORM_WIDTH / 2.f);
    std::uniform_real_distribution<float> chance(0.f, 1.f);

    float x = xDist(rng);
    float roll = chance(rng);

    // FIX: previously this function never checked isPositionValid, so a new
    // platform could be placed directly on top of an existing platform,
    // monster, or hole. Resample the x position (keeping the same roll/type)
    // until the candidate bounds are free, or give up after a few tries.
    constexpr int MAX_PLACEMENT_ATTEMPTS = 12;
    for (int attempt = 0; attempt < MAX_PLACEMENT_ATTEMPTS; ++attempt) {
        sf::FloatRect candidate(x - Constants::PLATFORM_WIDTH / 2.f,
                                 y - Constants::PLATFORM_HEIGHT / 2.f,
                                 Constants::PLATFORM_WIDTH, Constants::PLATFORM_HEIGHT);
        if (isPositionValid(candidate)) break;
        x = xDist(rng);
    }

    auto& textures = ResourceManager<sf::Texture>::instance();
    std::unique_ptr<Platform> platform;
    outIsBreakable = false;

    if (!forceSolid && !previous_broken && roll < Constants::BREAKABLE_CHANCE) {
        platform = std::make_unique<BreakablePlatform>(
            textures.get("assets/broken_platform.png"), sf::Vector2f(x, y));
        previous_broken = true;
        outIsBreakable = true;
    } else if (roll < Constants::BREAKABLE_CHANCE + Constants::MOVING_CHANCE) {
        // Select moving platform speed based on difficulty level
        float movingSpeed = Constants::MOVING_PLATFORM_SPEED_EASY;
        if (difficulty == Difficulty::Medium) {
            movingSpeed = Constants::MOVING_PLATFORM_SPEED_MEDIUM;
        } else if (difficulty == Difficulty::Hard) {
            movingSpeed = Constants::MOVING_PLATFORM_SPEED_HARD;
        }

        platform = std::make_unique<MovingPlatform>(
            textures.get("assets/moving_platform.png"), sf::Vector2f(x, y),
            windowWidth, movingSpeed);
        previous_broken = false;
    } else {
        platform = std::make_unique<NormalPlatform>(
            textures.get("assets/normal_platform.png"), sf::Vector2f(x, y));

        if (chance(rng) < Constants::SPRING_CHANCE) {
            platform->attachSpring(textures.get("assets/spring_sprite.png"));
        }
        previous_broken = false;
    }

    return platform;
}

void PlatformManager::trySpawnMonsterOrHole(float y, Difficulty difficulty, float playerClimbingScore) {
    if (playerClimbingScore < 600.f) return;

    std::uniform_real_distribution<float> chance(0.f, 1.f);
    std::uniform_real_distribution<float> xDist(60.f, windowWidth - 60.f);
    auto& textures = ResourceManager<sf::Texture>::instance();

    float monsterChance = (difficulty == Difficulty::Easy) ? Constants::MONSTER_SPAWN_CHANCE_EASY :
                          (difficulty == Difficulty::Medium) ? Constants::MONSTER_SPAWN_CHANCE_MEDIUM :
                                                               Constants::MONSTER_SPAWN_CHANCE_HARD;

    int monsterHP = (difficulty == Difficulty::Easy) ? Constants::MONSTER_HP_EASY :
                    (difficulty == Difficulty::Medium) ? Constants::MONSTER_HP_MEDIUM :
                                                         Constants::MONSTER_HP_HARD;

    if (chance(rng) < monsterChance) {
        float x = xDist(rng);
        sf::FloatRect bounds(x - Constants::MONSTER_WIDTH / 2.f, y - Constants::MONSTER_HEIGHT / 2.f, 
                             Constants::MONSTER_WIDTH, Constants::MONSTER_HEIGHT);

        if (isPositionValid(bounds)) {
            // Choose between blue (two textures) or green (same texture)
            if (chance(rng) > 0.5f) {
                monsters.push_back(std::make_unique<Monster>(
                    textures.get("assets/BlueMonsterL.png"),
                    textures.get("assets/BlueMonsterR.png"),
                    sf::Vector2f(x, y),
                    monsterHP
                ));
            } else {
                // Green monster uses the same texture for both sides
                monsters.push_back(std::make_unique<Monster>(
                    textures.get("assets/green_monsterL.png"),
                    textures.get("assets/green_monster.png"),
                    sf::Vector2f(x, y),
                    monsterHP
                ));
            }
        }
    } 
    else if (difficulty == Difficulty::Hard && chance(rng) < Constants::BIG_HOLE_SPWAN_CHANCE_HARD) {
        float x = xDist(rng);
        float sz = Constants::HOLE_LARGE_SIZE;
        sf::FloatRect bounds(x - sz / 2.f, y - sz / 2.f, sz+4.f, sz+4.f);
        if (isPositionValid(bounds)) {
            holes.push_back(std::make_unique<Hole>(textures.get("assets/hole@2x.png"), sf::Vector2f(x, y), sz));
        }
    }
    else if (difficulty == Difficulty::Hard && chance(rng) < Constants::HOLE_SPAWN_CHANCE_HARD) {
        float x = xDist(rng);
        float sz = Constants::HOLE_SMALL_SIZE;
        sf::FloatRect bounds(x - sz / 2.f, y - sz / 2.f, sz+4.f, sz+4.f);
        if (isPositionValid(bounds)) {
            holes.push_back(std::make_unique<Hole>(textures.get("assets/hole.png"), sf::Vector2f(x, y), sz));
        }
    }
}

void PlatformManager::spawnInitial(Difficulty difficulty) {
    auto& textures = ResourceManager<sf::Texture>::instance();
    platforms.push_back(std::make_unique<NormalPlatform>(
        textures.get("assets/normal_platform.png"),
        sf::Vector2f(windowWidth / 2.f, windowHeight - 60.f)));

    highestY = windowHeight - 60.f;
    lastSolidY = highestY;

    while (highestY > 0.f) {
        std::uniform_real_distribution<float> gapDist(Constants::MIN_VERTICAL_GAP, Constants::MAX_VERTICAL_GAP);
        float nextY = highestY - gapDist(rng);
        bool forceSolid = false;

        if ((lastSolidY - nextY) > Constants::MAX_VERTICAL_GAP) {
            nextY = lastSolidY - Constants::MAX_VERTICAL_GAP;
            forceSolid = true;
        }

        highestY = nextY;
        bool isBreakable = false;
        platforms.push_back(createRandomPlatform(highestY, forceSolid, isBreakable, difficulty));
        if (!isBreakable) lastSolidY = highestY;
    }
}

void PlatformManager::maintain(Difficulty difficulty, float playerClimbingScore) {
    while (highestY > -Constants::MAX_VERTICAL_GAP) {
        std::uniform_real_distribution<float> gapDist(Constants::MIN_VERTICAL_GAP, Constants::MAX_VERTICAL_GAP);
        float nextY = highestY - gapDist(rng);
        bool forceSolid = false;

        if ((lastSolidY - nextY) > Constants::MAX_VERTICAL_GAP) {
            nextY = lastSolidY - Constants::MAX_VERTICAL_GAP;
            forceSolid = true;
        }

        highestY = nextY;
        bool isBreakable = false;
        platforms.push_back(createRandomPlatform(highestY, forceSolid, isBreakable, difficulty));

        if (!isBreakable) {
            lastSolidY = highestY;
            trySpawnMonsterOrHole(highestY - 20.f, difficulty, playerClimbingScore);
        }
    }

    // Prune off-screen entities
    platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
        [this](const std::unique_ptr<Platform>& p) { return p->getPosition().y > windowHeight + 60.f; }),
        platforms.end());

    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
        [this](const std::unique_ptr<Monster>& m) { return m->getPosition().y > windowHeight + 60.f || m->isDead(); }),
        monsters.end());

    holes.erase(std::remove_if(holes.begin(), holes.end(),
        [this](const std::unique_ptr<Hole>& h) { return h->getCenterPosition().y > windowHeight + 60.f; }),
        holes.end());
}

void PlatformManager::update(float dt) {
    for (auto& platform : platforms) platform->update(dt);
    for (auto& monster : monsters) monster->update(dt, windowWidth);
}

void PlatformManager::scroll(float dy) {
    for (auto& platform : platforms) platform->move(0.f, dy);
    for (auto& monster : monsters) monster->move(0.f, dy);
    for (auto& hole : holes) hole->move(0.f, dy);

    highestY += dy;
    lastSolidY += dy;
}

void PlatformManager::render(sf::RenderWindow& window) {
    for (auto& platform : platforms) platform->render(window);
    for (auto& hole : holes) hole->render(window);
    for (auto& monster : monsters) monster->render(window);
}
