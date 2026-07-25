#include "Game.hpp"
#include "ResourceManager.hpp"
#include "BreakablePlatform.hpp"
#include "Constants.hpp"
#include <algorithm>
#include <cmath>

Game::Game()
    : window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT),
             Constants::WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close) {
    window.setFramerateLimit(60);

    auto& textures = ResourceManager<sf::Texture>::instance();

    // 1. Instantiate Player and PlatformManager
    player = std::make_unique<Player>(
        textures.get("assets/left_doodle.png"),
        textures.get("assets/right_doodle.png"),
        textures.get("assets/Shooting@Pose.png"),
        textures.get("assets/Nose2.png"),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.f, Constants::WINDOW_HEIGHT - 100.f));

    platformManager = std::make_unique<PlatformManager>(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);

    // 2. Setup Background
    background.setTexture(textures.get("assets/background.png"));
    sf::FloatRect bgBounds = background.getLocalBounds();
    if (bgBounds.width > 0.f && bgBounds.height > 0.f) {
        background.setScale(static_cast<float>(Constants::WINDOW_WIDTH) / bgBounds.width,
                            static_cast<float>(Constants::WINDOW_HEIGHT) / bgBounds.height);
    }

    // 3. UI Font & Texts
    font.loadFromFile("fonts/ariblk.ttf");

    titleText.setFont(font);
    titleText.setString("DOODLE JUMP");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Black);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    titleText.setPosition(Constants::WINDOW_WIDTH / 2.f, 80.f);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(18);
    highScoreText.setFillColor(sf::Color(40, 40, 40));

    gameModeText.setFont(font);
    gameModeText.setCharacterSize(18);
    gameModeText.setFillColor(sf::Color::Blue);

    scoreText.setFont(font);
    scoreText.setCharacterSize(22);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10.f, 10.f);

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(38);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect gb = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gb.width / 2.f, gb.height / 2.f);
    gameOverText.setPosition(Constants::WINDOW_WIDTH / 2.f, 120.f);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(20);
    finalScoreText.setFillColor(sf::Color::Black);

    volumeText.setFont(font);
    volumeText.setCharacterSize(20);
    volumeText.setFillColor(sf::Color::Black);

    popup.setFont(font);
    popup.setPosition(180.f, 10.f);
    popup.setCharacterSize(20);
    popup.setColor(sf::Color::Green);

    // Difficulty selector boxes (Easy / Medium / Hard) laid out in a row
    {
        const char* labels[NUM_DIFFICULTIES] = {"Easy", "Medium", "Hard"};
        const float boxWidth = 90.f;
        const float boxHeight = 42.f;
        const float spacing = 14.f;
        const float totalWidth = NUM_DIFFICULTIES * boxWidth + (NUM_DIFFICULTIES - 1) * spacing;
        const float startX = Constants::WINDOW_WIDTH / 2.f - totalWidth / 2.f;
        const float boxY = 330.f;

        for (int i = 0; i < NUM_DIFFICULTIES; ++i) {
            difficultyBoxes[i].setSize(sf::Vector2f(boxWidth, boxHeight));
            difficultyBoxes[i].setOutlineThickness(2.f);
            difficultyBoxes[i].setOutlineColor(sf::Color::Blue);
            difficultyBoxes[i].setPosition(startX + i * (boxWidth + spacing), boxY);

            difficultyLabels[i].setFont(font);
            difficultyLabels[i].setString(labels[i]);
            difficultyLabels[i].setCharacterSize(18);
        }
    }

    // 4. Sprites & Buttons
    startButton.setTexture(textures.get("assets/start_button.png"));
    restartButton.setTexture(textures.get("assets/restart_button.png"));
    menuButton.setTexture(textures.get("assets/menu_button.png"));
    settingsButton.setTexture(textures.get("assets/Settings_button.png"));
    backButton.setTexture(textures.get("assets/back_button.png"));

    auto centerOriginAndPlace = [](sf::Sprite& s, float y) {
        sf::FloatRect b = s.getLocalBounds();
        s.setOrigin(b.width / 2.f, b.height / 2.f);
        s.setPosition(Constants::WINDOW_WIDTH / 2.f, y);
    };

    centerOriginAndPlace(startButton, 340.f);
    centerOriginAndPlace(settingsButton, 450.f);
    centerOriginAndPlace(backButton, 550.f);
    centerOriginAndPlace(restartButton, 335.f);
    centerOriginAndPlace(menuButton, 435.f);

    // 5. Volume Slider UI Setup
    sliderBar.setSize(sf::Vector2f(200.f, 10.f));
    sliderBar.setFillColor(sf::Color(150, 150, 150));
    sliderBar.setPosition(Constants::WINDOW_WIDTH / 2.f - 100.f, 250.f);

    sliderKnob.setRadius(12.f);
    sliderKnob.setFillColor(sf::Color::Blue);
    sliderKnob.setOrigin(12.f, 12.f);

    gameOverOverlay.setSize(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
    gameOverOverlay.setFillColor(sf::Color(255, 255, 255, 150)); // semi‑transparent white

    // 6. Sound & Audio Initialization
    auto& soundManager = SoundManager::instance();
    
    // Preload sound buffers into ResourceManager to eliminate frame drops on first play
    soundManager.preloadSound("sounds/Shooting_Sound.wav");
    soundManager.preloadSound("sounds/Jumping_Sound.wav");
    soundManager.preloadSound("sounds/Loosing_Sound.wav");

    soundManager.loadBGM("sounds/MainMenu_Song.flac");
    soundManager.setVolume(settingsManager.getVolume());
    soundManager.playBGM();
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
            settingsManager.save();
            window.close();
        } else if (event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                  static_cast<float>(event.mouseButton.y));
            if (state == GameState::Menu) {
                handleMenuClick(mousePos);
            } else if (state == GameState::Settings) {
                // Grabbing the knob (or clicking anywhere on the bar) starts a smooth drag
                if (sliderKnob.getGlobalBounds().contains(mousePos) ||
                    sliderBar.getGlobalBounds().contains(mousePos)) {
                    isDraggingSlider = true;
                    updateSliderFromMouseX(mousePos.x);
                } else {
                    handleSettingsClick(mousePos);
                }
            } else if (state == GameState::GameOver) {
                handleGameOverClick(mousePos);
            }
        } else if (event.type == sf::Event::MouseButtonReleased &&
                   event.mouseButton.button == sf::Mouse::Left) {
            isDraggingSlider = false;
        } else if (event.type == sf::Event::MouseMoved) {
            if (state == GameState::Settings && isDraggingSlider) {
                updateSliderFromMouseX(static_cast<float>(event.mouseMove.x));
            }
        }
    }
}

void Game::updateSliderFromMouseX(float mouseX) {
    float relX = mouseX - sliderBar.getPosition().x;
    float newVol = std::clamp((relX / sliderBar.getSize().x) * 100.f, 0.f, 100.f);
    settingsManager.setVolume(newVol);
    SoundManager::instance().setVolume(newVol);

    // Update knob position immediately so dragging feels smooth (not tied to render's normal update path)
    sliderKnob.setPosition(sliderBar.getPosition().x + (newVol / 100.f) * sliderBar.getSize().x,
                           sliderBar.getPosition().y + 5.f);
}

void Game::fireBullet() {
    bullets.push_back(std::make_unique<Bullet>(player->getSnoutPosition()));
    SoundManager::instance().playSound("sounds/Shooting_Sound.wav");
}

void Game::update(float dt) {
    if (state != GameState::Playing) {
        SoundManager::instance().playBGM();
        return; // Pause gameplay physics outside Playing state
    }

    // Stop background music during gameplay
    SoundManager::instance().stopBGM();

    // Hole Sucking Transition Physics
    if (isBeingSuckedIn) {
        player->setScale(player->getScale() - (Constants::PLAYER_SHRINK_SPEED / 100.f) * dt);
        sf::Vector2f dir = targetHolePos - player->getPosition();
        player->setPosition(player->getPosition() + dir * dt * 5.f);

        if (player->getScale() <= 0.05f) {
            isBeingSuckedIn = false;
            state = GameState::GameOver;
            SoundManager::instance().playSound("sounds/Loosing_Sound.wav");
            settingsManager.updateHighScore(player->getScore());
            settingsManager.save();
        }
        return; // Pause all other gameplay physics during hole animation
    }

    player->handleInput();
    player->update(dt, Constants::WINDOW_WIDTH);

    // Controlled shooting rate according to difficulty
    fireTimer += dt;
    float cooldown = (settingsManager.getDifficulty() == Difficulty::Easy) ? 0.18f : 0.35f;
    if (player->isShooting() && fireTimer >= cooldown) {
        fireBullet();
        fireTimer = 0.f;
    }

    // Update Bullet Movement
    for (auto& b : bullets) {
        b->update(dt);
    }

    // Bullet-Monster Collisions & Memory Clean-up
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [this](const std::unique_ptr<Bullet>& b) {
            if (b->isOffScreen()) return true;

            for (auto& m : platformManager->getMonsters()) {
                if (!m->isDead() && m->getBounds().intersects(b->getBounds())) {
                    m->takeDamage(1);
                    if(m->isDead()){ player->addScore(200.f); popup.show("+200 points!", 2.0f);}
                    return true; // Delete bullet on hit
                }
            }
            return false;
        }), bullets.end());

    platformManager->update(dt);
    platformManager->maintain(settingsManager.getDifficulty(), static_cast<float>(player->getScore()));

    checkCollisions();

    // Endless Upward Scrolling
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
    sf::FloatRect pBounds = player->getBounds();

    // 1. Hole Collision Check (Highest Priority in Hard Difficulty)
    if (settingsManager.getDifficulty() == Difficulty::Hard) {
        for (auto& h : platformManager->getHoles()) {
            if (h->getBounds().intersects(pBounds)) {
                isBeingSuckedIn = true;
                targetHolePos = h->getCenterPosition();
                return;
            }
        }
    }

    // 2. Monster Collision Check
    for (auto& m : platformManager->getMonsters()) {
        if (m->isDead()) continue;

        sf::FloatRect mBounds = m->getBounds();
        if (pBounds.intersects(mBounds)) {
            // Landing on top surface of a monster
            if (player->isFalling() && (pBounds.top + pBounds.height) <= mBounds.top + 20.f) {
                player->applyVelocityY(Constants::MONSTER_BOUNCE_VEL);
                SoundManager::instance().playSound("sounds/Jumping_Sound.wav");
                m->takeDamage(99); // Instantly defeat monster on top bounce
                player->addScore(200.f);
                popup.show("+200 points!", 2.0f);
                return;
            } else {
                // Side or bottom contact causes game over
                state = GameState::GameOver;
                SoundManager::instance().playSound("sounds/Loosing_Sound.wav");
                settingsManager.updateHighScore(player->getScore());
                settingsManager.save();
                return;
            }
        }
    }

    // 3. Platform & Spring Collisions
    if (!player->isFalling()) return;

    float playerBottom = pBounds.top + pBounds.height;
    const float landingTolerance = 16.f;

    for (auto& platformPtr : platformManager->getPlatforms()) {
        Platform& platform = *platformPtr;
        if (!platform.isAlive()) continue;

        if (auto* breakable = dynamic_cast<BreakablePlatform*>(&platform)) {
            if (breakable->isBroken()) continue;
        }

        sf::FloatRect plBounds = platform.getBounds();
        bool horizontalOverlap = pBounds.left + pBounds.width > plBounds.left &&
                                 pBounds.left < plBounds.left + plBounds.width;

        // Spring Collision Priority over Plain Platform
        if (platform.hasSpring()) {
            sf::FloatRect sBounds = platform.getSpring()->getBounds();
            bool springOverlap = pBounds.left + pBounds.width > sBounds.left &&
                                 pBounds.left < sBounds.left + sBounds.width &&
                                 playerBottom >= sBounds.top &&
                                 playerBottom <= sBounds.top + landingTolerance;
            if (springOverlap) {
                player->applyVelocityY(Constants::SPRING_VELOCITY);
                SoundManager::instance().playSound("sounds/Jumping_Sound.wav");
                return;
            }
        }

        bool verticalOverlap = playerBottom >= plBounds.top &&
                               playerBottom <= plBounds.top + landingTolerance;

        if (horizontalOverlap && verticalOverlap) {
            platform.onLand(*player);
            SoundManager::instance().playSound("sounds/Jumping_Sound.wav");
            return;
        }
    }
}

void Game::checkGameOver() {
    sf::Vector2f pos = player->getPosition();
    if (pos.y - Constants::PLAYER_HEIGHT / 2.f > static_cast<float>(Constants::WINDOW_HEIGHT)) {
        state = GameState::GameOver;
        SoundManager::instance().playSound("sounds/Loosing_Sound.wav");
        settingsManager.updateHighScore(player->getScore());
        settingsManager.save();
    }
    popup.update();
}

void Game::render() {
    window.clear(sf::Color::White);
    window.draw(background);

    if (state == GameState::Menu) {
        window.draw(titleText);

        std::string diff;

        {switch (settingsManager.getDifficulty())
        {
        case Difficulty::Easy: diff = "EASY";
            break;
        
        case Difficulty::Medium: diff = "MEDIUM";
            break;

        default: diff = "HARD";
        }}

        gameModeText.setString("GameMode: " + diff);
        sf::FloatRect hb = gameModeText.getLocalBounds();
        gameModeText.setOrigin(hb.width / 2.f, hb.height / 2.f);
        gameModeText.setPosition(Constants::WINDOW_WIDTH / 2.f, 180.f);

        int currentHighScore = settingsManager.getCurrentHighScore();
        highScoreText.setString("HIGH SCORE: " + std::to_string(currentHighScore));
        hb = highScoreText.getLocalBounds();
        highScoreText.setOrigin(hb.width / 2.f, hb.height / 2.f);
        highScoreText.setPosition(Constants::WINDOW_WIDTH / 2.f, 250.f);

        window.draw(gameModeText);
        window.draw(highScoreText);
        window.draw(startButton);
        window.draw(settingsButton);
    } 
    else if (state == GameState::Settings) {
        window.draw(titleText);

        volumeText.setString("Volume: " + std::to_string(static_cast<int>(settingsManager.getVolume())) + "%");
        sf::FloatRect vb = volumeText.getLocalBounds();
        volumeText.setOrigin(vb.width / 2.f, vb.height / 2.f);
        volumeText.setPosition(Constants::WINDOW_WIDTH / 2.f, 210.f);

        Difficulty diff = settingsManager.getDifficulty();
        int selectedIndex = (diff == Difficulty::Easy) ? 0 : (diff == Difficulty::Medium) ? 1 : 2;

        for (int i = 0; i < NUM_DIFFICULTIES; ++i) {
            bool selected = (i == selectedIndex);
            difficultyBoxes[i].setFillColor(selected ? sf::Color::Blue : sf::Color::White);
            difficultyLabels[i].setFillColor(selected ? sf::Color::White : sf::Color::Blue);

            sf::FloatRect boxBounds = difficultyBoxes[i].getGlobalBounds();
            sf::FloatRect labelBounds = difficultyLabels[i].getLocalBounds();
            difficultyLabels[i].setOrigin(labelBounds.left + labelBounds.width / 2.f,
                                          labelBounds.top + labelBounds.height / 2.f);
            difficultyLabels[i].setPosition(boxBounds.left + boxBounds.width / 2.f,
                                            boxBounds.top + boxBounds.height / 2.f);
        }

        if (!isDraggingSlider) {
            sliderKnob.setPosition(sliderBar.getPosition().x + (settingsManager.getVolume() / 100.f) * sliderBar.getSize().x,
                                   sliderBar.getPosition().y + 5.f);
        }

        window.draw(volumeText);
        window.draw(sliderBar);
        window.draw(sliderKnob);
        for (int i = 0; i < NUM_DIFFICULTIES; ++i) {
            window.draw(difficultyBoxes[i]);
            window.draw(difficultyLabels[i]);
        }
        window.draw(backButton);
    } 
    else if (state == GameState::Playing) {
        platformManager->render(window);
        for (auto& b : bullets) b->render(window);
        player->render(window);
        window.draw(scoreText);
        popup.draw(window);
    } 
    else if (state == GameState::GameOver) {
        platformManager->render(window);
        player->render(window);

        window.draw(gameOverOverlay);

        int activeHighScore = settingsManager.getCurrentHighScore();

        finalScoreText.setString("Score: " + std::to_string(player->getScore()) +
                                 "\nHigh Score: " + std::to_string(activeHighScore));
        sf::FloatRect fb = finalScoreText.getLocalBounds();
        finalScoreText.setOrigin(fb.width / 2.f, fb.height / 2.f);
        finalScoreText.setPosition(Constants::WINDOW_WIDTH / 2.f, 220.f);

        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(restartButton);
        window.draw(menuButton);
    }

    window.display();
}

void Game::startNewGame() {
    bullets.clear();
    isBeingSuckedIn = false;
    player->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.f, Constants::WINDOW_HEIGHT - 100.f));
    player->applyVelocityY(0.f);
    player->resetScore();
    platformManager->reset(settingsManager.getDifficulty());
    state = GameState::Playing;
}

void Game::handleMenuClick(sf::Vector2f mousePos) {
    if (startButton.getGlobalBounds().contains(mousePos)) {
        startNewGame();
    } else if (settingsButton.getGlobalBounds().contains(mousePos)) {
        state = GameState::Settings;
    }
}

void Game::handleSettingsClick(sf::Vector2f mousePos) {
    if (backButton.getGlobalBounds().contains(mousePos)) {
        settingsManager.save();
        state = GameState::Menu;
        return;
    }

    for (int i = 0; i < NUM_DIFFICULTIES; ++i) {
        if (difficultyBoxes[i].getGlobalBounds().contains(mousePos)) {
            Difficulty target = (i == 0) ? Difficulty::Easy :
                                (i == 1) ? Difficulty::Medium : Difficulty::Hard;
            selectDifficulty(target);
            return;
        }
    }
}

void Game::selectDifficulty(Difficulty target) {
    // SettingsManager only exposes cycling, so step through until the target is reached
    for (int i = 0; i < NUM_DIFFICULTIES; ++i) {
        if (settingsManager.getDifficulty() == target) return;
        settingsManager.cycleDifficulty();
    }
}

void Game::handleGameOverClick(sf::Vector2f mousePos) {
    if (restartButton.getGlobalBounds().contains(mousePos)) {
        startNewGame();
    } else if (menuButton.getGlobalBounds().contains(mousePos)) {
        state = GameState::Menu;
    }
}