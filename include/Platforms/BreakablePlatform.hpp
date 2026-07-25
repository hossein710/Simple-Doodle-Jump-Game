#pragma once

#include "Platform.hpp"

// Breaks the moment the player lands on it: no jump is granted, and the
// platform (with the player riding it) starts falling. The player can still
// move left/right during the fall and may be rescued by landing on a
// neighbouring healthy platform.
class BreakablePlatform : public Platform {
public:
    BreakablePlatform(sf::Texture& texture, sf::Vector2f position);

    void update(float dt) override;
    void onLand(Player& player) override;

    bool isBroken() const { return broken; }

private:
    bool broken = false;
    float fallVelocity = 0.f;
};
