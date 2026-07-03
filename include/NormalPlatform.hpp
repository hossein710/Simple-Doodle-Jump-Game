#pragma once

#include "Platform.hpp"

// Stable, stationary platform — the standard jump base.
// All of its behaviour is already provided by Platform's defaults, so this
// class exists mainly to give it a distinct, identifiable type.
class NormalPlatform : public Platform {
public:
    NormalPlatform(sf::Texture& texture, sf::Vector2f position)
        : Platform(texture, position, PlatformType::Normal) {}
};
