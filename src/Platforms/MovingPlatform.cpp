#include "MovingPlatform.hpp"

MovingPlatform::MovingPlatform(sf::Texture& texture, sf::Vector2f position,
                                unsigned int windowWidth, float speed)
    : Platform(texture, position, PlatformType::Moving),
      direction(1.f), speed(speed), windowWidth(windowWidth) {isMoving = true;}

void MovingPlatform::update(float dt) {
    sf::FloatRect bounds = getBounds();

    if (bounds.left <= 0.f) {
        direction = 1.f;
    } else if (bounds.left + bounds.width >= static_cast<float>(windowWidth)) {
        direction = -1.f;
    }

    move(direction * speed * dt, 0.f);
}
