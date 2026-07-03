#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Platform.hpp"

// ---------------------------------------------------------------------------
// PlatformManager
// Owns every active platform. Responsible for the initial layout, endless
// randomized generation as the player climbs, scrolling the whole set down
// (the illusion of upward movement), and pruning platforms that scrolled
// off the bottom of the screen.
// ---------------------------------------------------------------------------
class PlatformManager {
public:
    PlatformManager(unsigned int windowWidth, unsigned int windowHeight);

    void reset();
    void update(float dt);
    void scroll(float dy); // moves every platform down by dy (world scroll)
    void render(sf::RenderWindow& window);

    // Generates new platforms above the current highest one whenever needed.
    void maintain();

    std::vector<std::unique_ptr<Platform>>& getPlatforms() { return platforms; }

private:
    void spawnInitial();

    // Creates a platform at height y. If forceSolid is true, the platform is
    // guaranteed not to be breakable (used when allowing a breakable would
    // push the gap since the last reliable platform beyond what's reachable).
    // Reports via outIsBreakable whether the created platform is a
    // BreakablePlatform, so callers can decide whether it's safe to treat as
    // the new "last solid" anchor for gap generation.
    std::unique_ptr<Platform> createRandomPlatform(float y, bool forceSolid, bool& outIsBreakable);

    unsigned int windowWidth;
    unsigned int windowHeight;

    std::vector<std::unique_ptr<Platform>> platforms;
    float highestY = 0.f;   // y coordinate of the topmost generated platform so far
    float lastSolidY = 0.f; // y coordinate of the topmost NON-breakable platform so far;
                             // gap generation is measured from this, not from highestY,
                             // so breakable platforms are never relied on as required
                             // stepping stones between reachable platforms.

    std::mt19937 rng;

    bool previous_broken = false;
};
