#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Monster {
public:
    Monster(sf::Texture& texL, sf::Texture& texR, sf::Vector2f position, int health);

    void update(float dt, unsigned int windowWidth);
    void render(sf::RenderWindow& window);
    void move(float dx, float dy);

    void takeDamage(int amount = 1);
    bool isDead() const { return health <= 0; }
    
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return active->getPosition(); }

private:
    sf::Sprite left;
    sf::Sprite right;
    sf::Sprite* active;   // pointer to the currently used sprite (left or right)
    int health;
    float speed = Constants::MONSTER_SPEED;
    int direction = 1; // 1 for right, -1 for left
};