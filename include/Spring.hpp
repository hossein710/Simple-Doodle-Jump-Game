#pragma once

#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------------------
// Spring
// A small power-up that can sit on top of a NormalPlatform. It is not a
// Platform itself (it never directly supports the player while idle) — it
// is a separate object owned by the platform, with its own bounds used by
// the shared collision system to detect a "player landed on spring" event.
// ---------------------------------------------------------------------------
class Spring {
public:
    Spring(sf::Texture& texture, sf::Vector2f position);

    void setPosition(sf::Vector2f position);
    void move(float dx, float dy);
    sf::Vector2f getPosition() const;

    sf::FloatRect getBounds() const;
    void render(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
};
