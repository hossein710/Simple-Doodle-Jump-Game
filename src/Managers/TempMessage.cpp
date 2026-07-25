#include "TempMessage.hpp"

TempMessage::TempMessage() {
    // Default styling (optional)
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);
    m_text.setString("");
}

void TempMessage::setFont(const sf::Font& font) {
    m_text.setFont(font);
}

void TempMessage::show(const std::string& text, float durationSeconds) {
    m_text.setString(text);
    m_duration = durationSeconds;
    m_clock.restart();      // start the timer
    m_visible = true;
}

void TempMessage::update() {
    if (m_visible && m_clock.getElapsedTime().asSeconds() >= m_duration) {
        m_visible = false;
        m_text.setString(""); // optional: clear the text
    }
}

void TempMessage::draw(sf::RenderWindow& window) const {
    if (m_visible) {
        window.draw(m_text);
    }
}

void TempMessage::setPosition(float x, float y) {
    // Center the text horizontally and vertically at (x, y)
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f,
                     bounds.top  + bounds.height / 2.f);
    m_text.setPosition(x, y);
}

void TempMessage::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
}

void TempMessage::setColor(const sf::Color& color) {
    m_text.setFillColor(color);
}