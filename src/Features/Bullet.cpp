#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f position) {
    // Set circle radius to half of the configured bullet width
    float radius = Constants::BULLET_WIDTH / 2.f; 
    shape.setRadius(radius);
    shape.setFillColor(sf::Color(255, 165, 0)); // Bright Orange
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::Black);

    // Center the origin so positioning matches the snout position
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
}

void Bullet::update(float dt) {
    shape.move(0.f, -Constants::BULLET_SPEED * dt);
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Bullet::isOffScreen() const {
    return shape.getPosition().y + Constants::BULLET_HEIGHT < 0.f;
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}