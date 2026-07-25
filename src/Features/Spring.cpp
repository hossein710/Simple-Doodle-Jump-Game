#include "Spring.hpp"
#include "Constants.hpp"

Spring::Spring(sf::Texture& texture, sf::Vector2f position) {
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width > 0.f && bounds.height > 0.f) {
        sprite.setScale(Constants::SPRING_WIDTH / bounds.width,
                         Constants::SPRING_HEIGHT / bounds.height);
    }
    sprite.setOrigin(bounds.width / 2.f, bounds.height);
    setPosition(position);
}

void Spring::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Spring::move(float dx, float dy) {
    sprite.move(dx, dy);
}

sf::Vector2f Spring::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Spring::getBounds() const {
    return sprite.getGlobalBounds();
}

void Spring::render(sf::RenderWindow& window) {
    window.draw(sprite);
}
