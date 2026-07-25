#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Hole {
public:
    Hole(sf::Texture& texture, sf::Vector2f position, float size);

    void render(sf::RenderWindow& window);
    void move(float dx, float dy);

    sf::FloatRect getBounds() const;
    sf::Vector2f getCenterPosition() const { return sprite.getPosition(); }

private:
    sf::Sprite sprite;
    float size;
};