#pragma once

#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "PlatformManager.hpp"

enum class GameState { Menu, Playing, GameOver };

// ---------------------------------------------------------------------------
// Game
// Owns the window and drives the three-phase game loop (events / update /
// render). Also owns the finite state machine (Menu -> Playing -> GameOver
// -> Playing|Menu) and all collision handling, since collisions are the one
// place where the player and the platform system need to talk to each other.
// ---------------------------------------------------------------------------
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

    void loadHighScore();
    void saveHighScoreIfNeeded();

    void handleMenuClick(sf::Vector2f mousePos);
    void handleGameOverClick(sf::Vector2f mousePos);
    
    sf::RenderWindow window;
    sf::Clock clock;

    GameState state = GameState::Menu;

    std::unique_ptr<Player> player;
    std::unique_ptr<PlatformManager> platformManager;

    int highScore = 0;

    // UI
    sf::Font font;
    sf::Text titleText;
    sf::Text highScoreText;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text finalScoreText;

    sf::Sprite startButton;
    sf::Sprite restartButton;
    sf::Sprite menuButton;
    sf::Sprite background;
};
