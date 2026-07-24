#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Bullet {
public:
    explicit Bullet(sf::Vector2f position);

    void update(float dt);
    void render(sf::RenderWindow& window);

    bool isOffScreen() const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
};