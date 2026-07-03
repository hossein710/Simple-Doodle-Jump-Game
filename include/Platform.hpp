#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Spring.hpp"

class Player; // forward declaration

enum class PlatformType { Normal, Moving, Breakable };

// ---------------------------------------------------------------------------
// Platform (abstract base class)
// All platform types inherit shared behaviour: position management and
// rendering. Each subtype only overrides the behaviour that differs:
// update() (e.g. moving back and forth) and onLand() (what happens when the
// player lands on it). The collision system in Game/PlatformManager always
// goes through this common interface, never through type-specific code.
// ---------------------------------------------------------------------------
class Platform {
public:
    Platform(sf::Texture& texture, sf::Vector2f position, PlatformType type);
    virtual ~Platform() = default;

    virtual void update(float /*dt*/) {}           // overridden by MovingPlatform / BreakablePlatform
    virtual void render(sf::RenderWindow& window);
    virtual void onLand(Player& player);           // shared landing behaviour, overridden by Breakable

    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;
    void move(float dx, float dy);

    sf::FloatRect getBounds() const;
    PlatformType getType() const { return type; }

    bool isAlive() const { return alive; }
    void kill() { alive = false; }

    // Springs only ever attach to Normal platforms, but the field lives on
    // the base class so the shared collision system can check any platform
    // through one consistent interface.
    void attachSpring(sf::Texture& springTexture);
    bool hasSpring() const { return spring != nullptr; }
    Spring* getSpring() { return spring.get(); }

protected:
    sf::Sprite sprite;
    PlatformType type;
    bool alive = true;
    std::unique_ptr<Spring> spring;
};
