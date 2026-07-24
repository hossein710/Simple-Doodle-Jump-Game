#include "Monster.hpp"

Monster::Monster(sf::Texture& texL, sf::Texture& texR, sf::Vector2f position, int health)
    : health(health) {
    left.setTexture(texL);
    right.setTexture(texR);

    // Scale and center both sprites
    sf::FloatRect b = left.getLocalBounds();
    if (b.width > 0.f && b.height > 0.f) {
        float sx = Constants::MONSTER_WIDTH / b.width;
        float sy = Constants::MONSTER_HEIGHT / b.height;
        left.setScale(sx, sy);
        right.setScale(sx, sy);
    }
    left.setOrigin(b.width / 2.f, b.height / 2.f);
    right.setOrigin(b.width / 2.f, b.height / 2.f);
    left.setPosition(position);
    right.setPosition(position);

    // Initially face right
    active = &right;
}

void Monster::update(float dt, unsigned int windowWidth) {
    // Choose active sprite based on direction
    active = (direction == 1) ? &right : &left;

    // Move the active sprite (both sprites share the same position, but we move only the active one)
    active->move(speed * direction * dt, 0.f);

    // Keep the other sprite in sync (so both sprites have the same position)
    sf::Vector2f pos = active->getPosition();
    if (direction == 1) {
        left.setPosition(pos);
    } else {
        right.setPosition(pos);
    }

    // Screen bounds reversal
    float halfWidth = Constants::MONSTER_WIDTH / 2.f;
    if (pos.x + halfWidth >= static_cast<float>(windowWidth)) {
        direction = -1;
        pos.x = static_cast<float>(windowWidth) - halfWidth; // clamp
    } else if (pos.x - halfWidth <= 0.f) {
        direction = 1;
        pos.x = halfWidth; // clamp
    }
    // Re-apply clamped position to both sprites
    left.setPosition(pos);
    right.setPosition(pos);
}

void Monster::render(sf::RenderWindow& window) {
    if (!isDead()) {
        window.draw(*active);
    }
}

void Monster::move(float dx, float dy) {
    // Move both sprites together
    left.move(dx, dy);
    right.move(dx, dy);
}

void Monster::takeDamage(int amount) {
    health -= amount;
}

sf::FloatRect Monster::getBounds() const {
    return active->getGlobalBounds();
}