#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.hpp"
#include "PlatformManager.hpp"
#include "Bullet.hpp"
#include "SoundManager.hpp"
#include "SettingsManager.hpp"
#include "Constants.hpp"
#include "TempMessage.hpp"

enum class GameState { Menu, Settings, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void startNewGame();
    void checkCollisions();
    void checkGameOver();

    void handleMenuClick(sf::Vector2f mousePos);
    void handleSettingsClick(sf::Vector2f mousePos);
    void handleGameOverClick(sf::Vector2f mousePos);

    void fireBullet();
    void selectDifficulty(Difficulty target);
    void updateSliderFromMouseX(float mouseX);

    sf::RenderWindow window;
    sf::Clock clock;

    GameState state = GameState::Menu;
    SettingsManager settingsManager;

    std::unique_ptr<Player> player;
    std::unique_ptr<PlatformManager> platformManager;
    std::vector<std::unique_ptr<Bullet>> bullets;

    // Hole Animation State
    bool isBeingSuckedIn = false;
    sf::Vector2f targetHolePos;

    // Fire rate control
    float fireTimer = 0.f;

    // UI Elements
    sf::Font font;
    sf::Text titleText;
    sf::Text highScoreText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text finalScoreText;
    sf::Text volumeText;
    sf::Text gameModeText;

    // Difficulty selector (3 boxes in a row)
    static constexpr int NUM_DIFFICULTIES = 3;
    sf::RectangleShape difficultyBoxes[NUM_DIFFICULTIES];
    sf::Text difficultyLabels[NUM_DIFFICULTIES];

    sf::Sprite startButton;
    sf::Sprite restartButton;
    sf::Sprite menuButton;
    sf::Sprite settingsButton;
    sf::Sprite backButton;
    sf::Sprite background;

    // Volume Slider UI
    sf::RectangleShape sliderBar;
    sf::CircleShape sliderKnob;
    bool isDraggingSlider = false;

    sf::RectangleShape gameOverOverlay;

    TempMessage popup;
};