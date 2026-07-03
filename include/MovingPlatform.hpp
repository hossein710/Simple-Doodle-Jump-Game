#pragma once

#include "Platform.hpp"

// Moves horizontally and bounces back at the edges of the screen.
class MovingPlatform : public Platform {
public:
    MovingPlatform(sf::Texture& texture, sf::Vector2f position, unsigned int windowWidth, float speed);

    void update(float dt) override;

private:
    float direction; // +1 or -1
    float speed;
    unsigned int windowWidth;
};
