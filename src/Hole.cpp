#include "Hole.hpp"

Hole::Hole(sf::Texture& texture, sf::Vector2f position, float size)
    : size(size) {
    sprite.setTexture(texture);
    sf::FloatRect b = sprite.getLocalBounds();
    if (b.width > 0.f && b.height > 0.f) {
        sprite.setScale(size / b.width, size / b.height);
    }
    sprite.setOrigin(b.width / 2.f, b.height / 2.f);
    sprite.setPosition(position);
}

void Hole::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Hole::move(float dx, float dy) {
    sprite.move(dx, dy);
}

sf::FloatRect Hole::getBounds() const {
    return sprite.getGlobalBounds();
}