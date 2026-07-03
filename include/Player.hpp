#pragma once

#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------------------
// Player
// The controllable character. Keeps its own position/velocity/score and
// handles input, gravity and screen wrapping. Jumping itself is triggered
// from the outside (by Platform::onLand / the spring collision check),
// since "when" a jump happens depends on what the player landed on.
// ---------------------------------------------------------------------------
class Player {
public:
    Player(sf::Texture& leftTexture, sf::Texture& rightTexture, sf::Vector2f startPosition);

    void handleInput();
    void update(float dt, unsigned int windowWidth);
    void render(sf::RenderWindow& window);

    void applyVelocityY(float vy); // used for normal jump / spring jump
    void moveDown(float dy);       // dragged down while riding a breaking platform

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);

    float getVelocityY() const { return velocity.y; }
    bool isFalling() const { return velocity.y > 0.f; }

    int getScore() const { return score; }
    void addScore(float worldScrollAmount);
    void resetScore();

private:
    sf::Sprite sprite;
    sf::Texture& texLeft;
    sf::Texture& texRight;

    sf::Vector2f velocity{0.f, 0.f};
    float scoreAccumulator = 0.f; // sub-integer accumulation of climbed height
    int score = 0;
};
