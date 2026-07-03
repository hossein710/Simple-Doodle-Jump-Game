#include "Player.hpp"
#include "Constants.hpp"

Player::Player(sf::Texture& leftTexture, sf::Texture& rightTexture, sf::Vector2f startPosition)
    : texLeft(leftTexture), texRight(rightTexture) {
    sprite.setTexture(texRight);
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width > 0.f && bounds.height > 0.f) {
        sprite.setScale(Constants::PLAYER_WIDTH / bounds.width,
                         Constants::PLAYER_HEIGHT / bounds.height);
    }
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    setPosition(startPosition);
}

void Player::handleInput() {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x -= Constants::PLAYER_SPEED;
        sprite.setTexture(texLeft);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x += Constants::PLAYER_SPEED;
        sprite.setTexture(texRight);
    }
}

void Player::update(float dt, unsigned int windowWidth) {
    // Gravity: continuously increases downward velocity, frame-independent.
    velocity.y += Constants::GRAVITY * dt;

    sprite.move(velocity.x * dt, velocity.y * dt);

    // Screen wrap: does not affect velocity, purely cosmetic teleport.
    sf::Vector2f pos = sprite.getPosition();
    float halfWidth = Constants::PLAYER_WIDTH / 2.f;
    if (pos.x + halfWidth < 0.f) {
        pos.x = static_cast<float>(windowWidth) + halfWidth;
        sprite.setPosition(pos);
    } else if (pos.x - halfWidth > static_cast<float>(windowWidth)) {
        pos.x = -halfWidth;
        sprite.setPosition(pos);
    }
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::applyVelocityY(float vy) {
    velocity.y = vy;
}

void Player::moveDown(float dy) {
    sprite.move(0.f, dy);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Player::addScore(float worldScrollAmount) {
    // Score represents total height climbed; accumulate fractional pixels
    // scrolled and convert to whole points.
    scoreAccumulator += worldScrollAmount;
    int gained = static_cast<int>(scoreAccumulator);
    score += gained;
    scoreAccumulator -= static_cast<float>(gained);
}

void Player::resetScore() {
    score = 0;
    scoreAccumulator = 0.f;
}
