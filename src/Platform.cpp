#include "Platform.hpp"
#include "Player.hpp"
#include "Constants.hpp"

Platform::Platform(sf::Texture& texture, sf::Vector2f position, PlatformType type)
    : type(type) {
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width > 0.f && bounds.height > 0.f) {
        sprite.setScale(Constants::PLATFORM_WIDTH / bounds.width,
                         Constants::PLATFORM_HEIGHT / bounds.height);
    }
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    setPosition(position);
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(sprite);
    if (spring) {
        spring->render(window);
    }
}

void Platform::onLand(Player& player) {
    // Default shared behaviour: a normal upward bounce.
    player.applyVelocityY(Constants::JUMP_VELOCITY);
}

void Platform::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
    if (spring) {
        sf::FloatRect bounds = getBounds();
        spring->setPosition({position.x, bounds.top});
    }
}

sf::Vector2f Platform::getPosition() const {
    return sprite.getPosition();
}

void Platform::move(float dx, float dy) {
    sprite.move(dx, dy);
    if (spring) {
        spring->move(dx, dy);
    }
}

sf::FloatRect Platform::getBounds() const {
    return sprite.getGlobalBounds();
}

void Platform::attachSpring(sf::Texture& springTexture) {
    if (type != PlatformType::Normal) {
        return; // springs only ever sit on normal platforms
    }
    sf::FloatRect bounds = getBounds();
    spring = std::make_unique<Spring>(springTexture, sf::Vector2f(getPosition().x, bounds.top));
}
