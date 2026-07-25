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
    Player(sf::Texture& leftTexture, sf::Texture& rightTexture, 
           sf::Texture& shootTex, sf::Texture& noseTex, sf::Vector2f startPosition);

    void handleInput();
    void update(float dt, unsigned int windowWidth);
    void render(sf::RenderWindow& window);

    void applyVelocityY(float vy); 
    void moveDown(float dy);       

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);

    float getVelocityY() const { return velocity.y; }
    bool isFalling() const { return velocity.y > 0.f; }

    int getScore() const { return score; }
    void addScore(float worldScrollAmount);
    void resetScore();

    // Shooting
    bool isShooting() const { return isShootingState; }
    sf::Vector2f getSnoutPosition() const;

    // Hole suck-in physics
    void setScale(float scaleFactor);
    float getScale() const { return currentScale; }

private:
    void updateNosePosition();

    sf::Sprite sprite;
    sf::Texture& texLeft;
    sf::Texture& texRight;
    sf::Texture& texShoot;

    // Nose overlay, only drawn while shooting
    sf::Sprite noseSprite;
    sf::Texture& texNose;

    sf::Vector2f velocity{0.f, 0.f};
    float scoreAccumulator = 0.f;
    int score = 0;

    bool isShootingState = false;
    bool facingRight = true;
    float currentScale = 1.0f;
};
