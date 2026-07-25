#include "BreakablePlatform.hpp"
#include "Player.hpp"
#include "Constants.hpp"

BreakablePlatform::BreakablePlatform(sf::Texture& texture, sf::Vector2f position)
    : Platform(texture, position, PlatformType::Breakable) {}

void BreakablePlatform::update(float dt) {
    if (broken) {
        fallVelocity += Constants::GRAVITY * dt;
        move(0.f, fallVelocity * dt);
    }
}

void BreakablePlatform::onLand(Player& player) {
    // No jump is granted; instead the platform starts falling under gravity
    // and the player (whose own upward bounce is cancelled here) falls in
    // sync with it, producing the "dragged down together" effect.
    broken = true;
    fallVelocity = 0.f;
    player.applyVelocityY(0.f);
}
