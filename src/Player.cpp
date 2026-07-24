#include "Player.hpp"
#include "Constants.hpp"

Player::Player(sf::Texture& leftTexture, sf::Texture& rightTexture, 
             sf::Texture& shootTex, sf::Texture& noseTex, sf::Vector2f startPosition)
    : texLeft(leftTexture), texRight(rightTexture), texShoot(shootTex), texNose(noseTex){
    sprite.setTexture(texRight);
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.width > 0.f && bounds.height > 0.f) {
        sprite.setScale(Constants::PLAYER_WIDTH / bounds.width, Constants::PLAYER_HEIGHT / bounds.height);
    }
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    setPosition(startPosition);

    noseSprite.setTexture(texNose);
    sf::FloatRect noseBounds = noseSprite.getLocalBounds();
    if (noseBounds.width > 0.f && noseBounds.height > 0.f) {
        noseSprite.setOrigin(noseBounds.width / 2.f, 0.f);
    }
}

void Player::handleInput() {
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x -= Constants::PLAYER_SPEED;
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x += Constants::PLAYER_SPEED;
        facingRight = true;
    }

    isShootingState = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    // Update texture based on facing direction and shooting state
    if (isShootingState) {
        sprite.setTexture(texShoot);
    } else {
        sprite.setTexture(facingRight ? texRight : texLeft);
    }
}

void Player::update(float dt, unsigned int windowWidth) {
    velocity.y += Constants::GRAVITY * dt;
    sprite.move(velocity.x * dt, velocity.y * dt);

    sf::Vector2f pos = sprite.getPosition();
    float halfWidth = (Constants::PLAYER_WIDTH * currentScale) / 2.f;
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

    if (isShootingState) {
        updateNosePosition();
        window.draw(noseSprite);
    }
}


void Player::updateNosePosition() {
    sf::FloatRect noseBounds = noseSprite.getLocalBounds();
    if (noseBounds.width <= 0.f || noseBounds.height <= 0.f) return;

    float scaleX = (Constants::NOSE_WIDTH / noseBounds.width) * currentScale;
    float scaleY = (Constants::NOSE_HEIGHT / noseBounds.height) * currentScale * 2;
    noseSprite.setScale(facingRight ? scaleX : -scaleX, scaleY);

    constexpr float NOSE_OFFSET_X = 0.f;
    constexpr float NOSE_OFFSET_Y = 0.f;
    sf::Vector2f snout = getSnoutPosition();
    snout.x += (facingRight ? NOSE_OFFSET_X : -NOSE_OFFSET_X) * currentScale - 3;
    snout.y += NOSE_OFFSET_Y * currentScale + 9;
    noseSprite.setPosition(snout);
}

void Player::applyVelocityY(float vy) { velocity.y = vy; }
void Player::moveDown(float dy) { sprite.move(0.f, dy); }

sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }
sf::Vector2f Player::getPosition() const { return sprite.getPosition(); }
void Player::setPosition(sf::Vector2f position) { sprite.setPosition(position); }

void Player::addScore(float worldScrollAmount) {
    scoreAccumulator += worldScrollAmount;
    int gained = static_cast<int>(scoreAccumulator);
    score += gained;
    scoreAccumulator -= static_cast<float>(gained);
}

void Player::resetScore() {
    score = 0;
    scoreAccumulator = 0.f;
    setScale(1.0f);
}

sf::Vector2f Player::getSnoutPosition() const {
    sf::Vector2f pos = getPosition();
    return sf::Vector2f(pos.x, pos.y - (Constants::PLAYER_HEIGHT * currentScale) / 2.f);
}

void Player::setScale(float scaleFactor) {
    currentScale = scaleFactor;
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setScale((Constants::PLAYER_WIDTH / bounds.width) * currentScale,
                    (Constants::PLAYER_HEIGHT / bounds.height) * currentScale);
}