#include "Game.hpp"
#include "ResourceManager.hpp"
#include "BreakablePlatform.hpp"
#include "Constants.hpp"
#include <fstream>
#include <string>

Game::Game()
    : window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT),
             Constants::WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close) {
    window.setFramerateLimit(60);

    auto& textures = ResourceManager<sf::Texture>::instance();

    player = std::make_unique<Player>(
        textures.get("assets/left_doodle.png"),
        textures.get("assets/right_doodle.png"),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.f, Constants::WINDOW_HEIGHT - 100.f));

    platformManager = std::make_unique<PlatformManager>(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);

    background.setTexture(textures.get("assets/background.png"));
    sf::FloatRect bgBounds = background.getLocalBounds();
    if (bgBounds.width > 0.f && bgBounds.height > 0.f) {
        background.setScale(static_cast<float>(Constants::WINDOW_WIDTH) / bgBounds.width,
                             static_cast<float>(Constants::WINDOW_HEIGHT) / bgBounds.height);
    }

    font.loadFromFile("fonts/ariblk.ttf");

    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(45);
    titleText.setFillColor(sf::Color::Black);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    titleText.setPosition(Constants::WINDOW_WIDTH / 2.f, 80.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(20);
    highScoreText.setFillColor(sf::Color(40, 40, 40));

    scoreText.setFont(font);
    scoreText.setCharacterSize(22);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 10.f);

    gameOverText.setFont(font);
    gameOverText.setString("YOU LOST!");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect gb = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gb.width / 2.f, gb.height / 2.f);
    gameOverText.setPosition(Constants::WINDOW_WIDTH / 2.f, 150.f);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(20);
    finalScoreText.setFillColor(sf::Color::Black);

    startButton.setTexture(textures.get("assets/start_button.png"));
    restartButton.setTexture(textures.get("assets/restart_button.png"));
    menuButton.setTexture(textures.get("assets/menu_button.png"));

    auto centerOriginAndPlace = [](sf::Sprite& s, float y) {
        sf::FloatRect b = s.getLocalBounds();
        s.setOrigin(b.width / 2.f, b.height / 2.f);
        s.setPosition(Constants::WINDOW_WIDTH / 2.f, y);
    };
    centerOriginAndPlace(startButton, 290.f);
    centerOriginAndPlace(restartButton, 335.f);
    centerOriginAndPlace(menuButton, 455.f);

    loadHighScore();
    highScoreText.setString("HIGH SCORE: " + std::to_string(highScore));
    sf::FloatRect hb = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hb.width / 2.f, hb.height / 2.f);
    highScoreText.setPosition(Constants::WINDOW_WIDTH / 2.f, 200.f);
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                   static_cast<float>(event.mouseButton.y));
            if (state == GameState::Menu) {
                handleMenuClick(mousePos);
            } else if (state == GameState::GameOver) {
                handleGameOverClick(mousePos);
            }
        }
    }
}

void Game::update(float dt) {
    if (state != GameState::Playing) {
        return; // gameplay update is fully paused outside the Playing state
    }

    player->handleInput();
    player->update(dt, Constants::WINDOW_WIDTH);
    platformManager->update(dt);

    checkCollisions();

    // Endless upward scroll illusion: keep the player pinned near mid-screen
    // and scroll the world (platforms) down instead of moving the player up.
    sf::Vector2f pos = player->getPosition();
    float center = Constants::WINDOW_HEIGHT / 2.f;
    if (pos.y < center) {
        float diff = center - pos.y;
        player->setPosition(sf::Vector2f(pos.x, center));
        platformManager->scroll(diff);
        player->addScore(diff);
    }

    scoreText.setString("Score: " + std::to_string(player->getScore()));

    checkGameOver();
}

void Game::checkCollisions() {
    if (!player->isFalling()) {
        return; // landing only ever happens while moving downward
    }

    sf::FloatRect pBounds = player->getBounds();
    float playerBottom = pBounds.top + pBounds.height;
    const float landingTolerance = 16.f;

    for (auto& platformPtr : platformManager->getPlatforms()) {
        Platform& platform = *platformPtr;
        if (!platform.isAlive()) {
            continue;
        }

        // A platform that already broke and is falling can no longer
        // support the player again.
        if (auto* breakable = dynamic_cast<BreakablePlatform*>(&platform)) {
            if (breakable->isBroken()) {
                continue;
            }
        }

        sf::FloatRect plBounds = platform.getBounds();
        bool horizontalOverlap = pBounds.left + pBounds.width > plBounds.left &&
                                  pBounds.left < plBounds.left + plBounds.width;

        // Spring check takes priority over the plain platform landing.
        if (platform.hasSpring()) {
            sf::FloatRect sBounds = platform.getSpring()->getBounds();
            bool springOverlap = pBounds.left + pBounds.width > sBounds.left &&
                                  pBounds.left < sBounds.left + sBounds.width &&
                                  playerBottom >= sBounds.top &&
                                  playerBottom <= sBounds.top + landingTolerance;
            if (springOverlap) {
                player->applyVelocityY(Constants::SPRING_VELOCITY);
                return;
            }
        }

        bool verticalOverlap = playerBottom >= plBounds.top &&
                                playerBottom <= plBounds.top + landingTolerance;

        if (horizontalOverlap && verticalOverlap) {
            // Shared collision response — every platform subtype (normal,
            // moving, breakable) is handled through the same onLand() call.
            platform.onLand(*player);
            return;
        }
    }
}

void Game::checkGameOver() {
    sf::Vector2f pos = player->getPosition();
    if (pos.y - Constants::PLAYER_HEIGHT / 2.f > static_cast<float>(Constants::WINDOW_HEIGHT)) {
        state = GameState::GameOver;
        saveHighScoreIfNeeded();

        finalScoreText.setString("\nScore: " + std::to_string(player->getScore()) +
                                  "\n\nHigh Score: " + std::to_string(highScore));
        sf::FloatRect fb = finalScoreText.getLocalBounds();
        finalScoreText.setOrigin(fb.width / 2.f, fb.height / 2.f);
        finalScoreText.setPosition(Constants::WINDOW_WIDTH / 2.f, 220.f);
    }
}

void Game::render() {
    window.clear(sf::Color::White);
    window.draw(background);

    if (state == GameState::Menu) {
        window.draw(titleText);
        window.draw(highScoreText);
        window.draw(startButton);
    } else if (state == GameState::Playing) {
        platformManager->render(window);
        player->render(window);
        window.draw(scoreText);
    } else if (state == GameState::GameOver) {
        platformManager->render(window);
        player->render(window);
        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(restartButton);
        window.draw(menuButton);
    }

    window.display();
}

void Game::startNewGame() {
    player->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.f, Constants::WINDOW_HEIGHT - 100.f));
    player->applyVelocityY(0.f);
    player->resetScore();
    platformManager->reset();
    state = GameState::Playing;
}

void Game::handleMenuClick(sf::Vector2f mousePos) {
    if (startButton.getGlobalBounds().contains(mousePos)) {
        startNewGame();
    }
}

void Game::handleGameOverClick(sf::Vector2f mousePos) {
    if (restartButton.getGlobalBounds().contains(mousePos)) {
        startNewGame();
    } else if (menuButton.getGlobalBounds().contains(mousePos)) {
        state = GameState::Menu;
        highScoreText.setString("HIGH SCORE: " + std::to_string(highScore));
        sf::FloatRect hb = highScoreText.getLocalBounds();
        highScoreText.setOrigin(hb.width / 2.f, hb.height / 2.f);
    }
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
    }
}

void Game::saveHighScoreIfNeeded() {
    int current = player->getScore();
    if (current > highScore) {
        highScore = current;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
        }
    }
}
