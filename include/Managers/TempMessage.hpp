#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// ---------------------------------------------------------------------------
// TempMessage
// Displays a text message for a given duration, then removes it automatically.
// Usage: call show() to start displaying, call update() each frame to check
//        the timer, and draw() if visible.
// ---------------------------------------------------------------------------
class TempMessage {
public:
    TempMessage();

    // Set the font to be used for the message.
    void setFont(const sf::Font& font);

    // Show a message for 'durationSeconds' seconds.
    void show(const std::string& text, float durationSeconds);

    // Call this every frame to update the timer and hide the message when time is up.
    void update();

    // Draw the message if it is currently visible.
    void draw(sf::RenderWindow& window) const;

    // Query whether the message is currently visible.
    bool isVisible() const { return m_visible; }

    // Convenience methods to style the text.
    void setPosition(float x, float y);
    void setCharacterSize(unsigned int size);
    void setColor(const sf::Color& color);

private:
    sf::Text m_text;
    sf::Clock m_clock;
    float m_duration = 0.f;
    bool m_visible = false;
};