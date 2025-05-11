#include "Game.hpp"
#include "../entities/PowerUp.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Duration (in seconds) for power-up effects
static constexpr float POWERUP_DURATION = 10.f;
// Angle offset in radians for multi-shot spread (e.g. 15°)
static constexpr float MULTI_SHOT_ANGLE = 15.f * 3.14159265f / 180.f;

Game::Game()
{
    // Score
    if (!font.loadFromFile("assets/ui/font.ttf"))
        std::cerr << "Error loading font!" << std::endl;

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 50.f);

    // Game over
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER\nPress Enter to return");
    gameOverText.setPosition(200.f, 300.f);
    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(32);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(200.f, 400.f);

    // Seed randomness
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Initialize shared power-up textures
    PowerUp::initTextures();

    // Load heart texture (lives)
    if (!heartTexture.loadFromFile("assets/ui/heart.png"))
        std::cerr << "Error loading heart texture!" << std::endl;
    heartSprite.setTexture(heartTexture);
    heartSprite.setScale(2.5f, 2.5f);

    // Load background
    if (!backgroundTexture.loadFromFile("assets/ui/background.png"))
        std::cerr << "Error loading background texture!" << std::endl;
    backgroundSprite.setTexture(backgroundTexture);

    // Load Earth frames
    for (int i = 0; i < EARTH_FRAMES; ++i) {
        std::string path = "assets/earth/4169310314-" + std::to_string(i) + ".png";
        if (!earthTextures[i].loadFromFile(path))
            std::cerr << "Error loading Earth frame " << i << std::endl;
    }
    earthSprite.setTexture(earthTextures[0]);
    auto eb = earthSprite.getLocalBounds();
    earthSprite.setOrigin(eb.width / 2.f, eb.height / 2.f);
}

void Game::handleInput(sf::RenderWindow& window)
{
    windowPtr = &window;
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
            backToMainMenu = true;

        if (gameOver && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
            backToMainMenu = true;
        }

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            float interval = rapidFireActive ? 0.1f : 0.25f;
            if (shootCooldown.getElapsedTime().asSeconds() > interval) {
                sf::Vector2f dir;
                if (player.shootDirection(dir)) {
                    // Multi-shot: spawn three bullets with angular offsets
                    if (multiShotActive) {
                        auto spawn = [&](float angleOffset) {
                            float ang = std::atan2(dir.y, dir.x) + angleOffset;
                            sf::Vector2f d = { std::cos(ang), std::sin(ang) };
                            bullets.emplace_back(player.getPosition(), d);
                            };
                        spawn(0.f);
                        spawn(MULTI_SHOT_ANGLE);
                        spawn(-MULTI_SHOT_ANGLE);
                    }
                    else {
                        // Single bullet (or laser behaves like a normal bullet but penetrates)
                        bullets.emplace_back(player.getPosition(), dir);
                    }

                    shootCooldown.restart();
                }
            }
        }
    }

    player.setWindow(&window);
    player.handleInput();
}

void Game::update()
{
    if (gameOver) return;

    if (!windowPtr) return;
    float dt = clock.restart().asSeconds();

    // Center & animate Earth
    centerEarthSprite(*windowPtr);
    updateEarthAnimation(dt);

    // Update game objects
    player.update();
    for (auto& a : enemies) a.update(dt);
    for (auto& b : bullets) b.update(dt);
    for (auto& pu : powerUps) pu.update(dt);

    // Handle power-up collection
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        if (it->getGlobalBounds().contains(player.getPosition())) {
            switch (it->getType()) {
            case PowerUpType::RapidFire:
                rapidFireActive = true;
                rapidFireTimer.restart();
                break;
            case PowerUpType::MultiShot:
                multiShotActive = true;
                multiShotTimer.restart();
                break;
            case PowerUpType::GainLife:
                lives = std::min(lives + 1, 5);
                break;
            }
            it = powerUps.erase(it);
        }
        else {
            ++it;
        }
    }

    // Expire power-ups after duration
    if (rapidFireActive && rapidFireTimer.getElapsedTime().asSeconds() >= POWERUP_DURATION)
        rapidFireActive = false;
    if (multiShotActive && multiShotTimer.getElapsedTime().asSeconds() >= POWERUP_DURATION)
        multiShotActive = false;


    // Spawn asteroids every 2 seconds
    spawnTimer += dt;
    if (spawnTimer >= 2.f) {
        spawnAstroid();
        spawnTimer -= 2.f;
    }

    // Spawn power-ups periodically
    if (spawnPowerUpClock.getElapsedTime().asSeconds() >= spawnPowerUpInterval) {
        spawnPowerUp();
        spawnPowerUpClock.restart();
    }

    // Bullet vs. asteroid collisions
    for (auto bit = bullets.begin(); bit != bullets.end();) {
        bool removed = false;
        for (auto eit = enemies.begin(); eit != enemies.end();) {
            if (bit->getGlobalBounds().intersects(eit->getGlobalBounds())) {
                eit = enemies.erase(eit);
                score += 10;
            }
            else {
                ++eit;
            }
        }
        if (!removed)
            ++bit;
    }

    // Asteroid vs. Earth collisions
    sf::FloatRect earthBounds = earthSprite.getGlobalBounds();
    for (auto eit = enemies.begin(); eit != enemies.end();) {
        if (eit->getGlobalBounds().intersects(earthBounds)) {
            --lives;
            eit = enemies.erase(eit);
        }
        else {
            ++eit;
        }
    }
    if (lives <= 0) {
        lives = 0;
        gameOver = true;

        finalScoreText.setString("Final Score: " + std::to_string(score));
    }

    // Remove off-screen bullets
    bullets.erase(
        std::remove_if(
            bullets.begin(), bullets.end(),
            [&](const Bullet& b) { return b.isOffScreen(windowPtr->getSize()); }
        ),
        bullets.end()
    );
}

void Game::draw(sf::RenderWindow& window)
{
    window.clear();
    scaleBackgroundToFit(window);
    window.draw(backgroundSprite);

    earthSprite.setScale(2.f, 2.f);
    window.draw(earthSprite);

    // Draw player & enemy
    window.draw(player);
    for (auto& a : enemies) window.draw(a);
    for (auto& b : bullets) window.draw(b);

    // Draw score
    scoreText.setString("Score: " + std::to_string(score));
    window.draw(scoreText);


    // Draw lives
    for (int i = 0; i < lives; ++i) {
        heartSprite.setPosition(
            10.f + i * (heartSprite.getGlobalBounds().width + 5.f),
            10.f
        );
        window.draw(heartSprite);
    }

    // Draw power-ups
    for (auto& pu : powerUps) window.draw(pu);

    // Draw gameover
    if (gameOver) {
        window.draw(gameOverText);
        window.draw(finalScoreText);
    }

    window.display();
}

void Game::spawnPowerUp()
{
    // spawn ring around Earth
    sf::Vector2f center = earthSprite.getPosition();
    float radius = earthSprite.getGlobalBounds().width * earthSprite.getScale().x / 2.f + 50.f;
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2 * 3.14159265f;
    float x = center.x + radius * std::cos(angle);
    float y = center.y + radius * std::sin(angle);
    PowerUpType type = static_cast<PowerUpType>(std::rand() % 4);
    powerUps.emplace_back(type, sf::Vector2f(x, y));
}

void Game::spawnAstroid()
{
    if (!windowPtr || lives <= 0) return;
    if ((std::rand() % 100) < 15)
        spawnAstroidCluster();
    else
        enemies.emplace_back(earthSprite.getPosition(), windowPtr->getSize());
}

void Game::spawnAstroidCluster()
{
    int clusterSize = std::rand() % 2 + 2;
    for (int i = 0; i < clusterSize; ++i) {
        sf::Vector2f offset(
            static_cast<float>(std::rand() % 200 - 100),
            static_cast<float>(std::rand() % 200 - 100)
        );
        enemies.emplace_back(
            earthSprite.getPosition() + offset,
            windowPtr->getSize()
        );
    }
}

void Game::scaleBackgroundToFit(const sf::RenderWindow& window)
{
    auto sz = window.getSize();
    backgroundSprite.setScale(
        float(sz.x) / backgroundTexture.getSize().x,
        float(sz.y) / backgroundTexture.getSize().y
    );
}

void Game::centerEarthSprite(const sf::RenderWindow& window)
{
    earthSprite.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f
    );
}

void Game::updateEarthAnimation(float dt)
{
    elapsedTime += dt;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % EARTH_FRAMES;
        earthSprite.setTexture(earthTextures[currentFrame]);
        elapsedTime -= frameTime;
    }
}
