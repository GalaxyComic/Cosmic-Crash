#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game()
{
    // Seed randomness
    std::srand(static_cast<unsigned>(std::time(nullptr)));

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
            std::cerr << "Error loading Earth frame " << i << "!" << std::endl;
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

        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            if (shootCooldown.getElapsedTime().asSeconds() > 0.25f) {
                sf::Vector2f dir;
                if (player.shootDirection(dir)) {
                    bullets.emplace_back(player.getPosition(), dir);
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
    if (!windowPtr) return;
    // Compute delta time
    float dt = clock.restart().asSeconds();

    // Re-center Earth & animate
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
            case PowerUpType::Laser:
                laserActive = true;
                //laserTimer.restart();
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
                bit = bullets.erase(bit);
                removed = true;
                break;
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
        backToMainMenu = true;
    }

    // Remove bullets that are off-screen
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

    // Draw background (scaled)
    scaleBackgroundToFit(window);
    window.draw(backgroundSprite);

    // Draw Earth (scaled up)
    earthSprite.setScale(2.f, 2.f);
    window.draw(earthSprite);

    // Draw player, enemies, bullets
    window.draw(player);
    for (auto& a : enemies) window.draw(a);
    for (auto& b : bullets) window.draw(b);

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

    window.display();
}

void Game::spawnPowerUp()
{
    PowerUpType type = static_cast<PowerUpType>(std::rand() % 4);
    float x = static_cast<float>(std::rand() % windowPtr->getSize().x);
    float y = static_cast<float>(std::rand() % windowPtr->getSize().y);
    powerUps.emplace_back(type, sf::Vector2f(x, y));
}

void Game::spawnAstroid()
{
    if (!windowPtr || lives <= 0)
        return;
    if ((std::rand() % 100) < 20)
        spawnAstroidCluster();
    else
        enemies.emplace_back(earthSprite.getPosition(), windowPtr->getSize());
}

void Game::spawnAstroidCluster()
{
    int clusterSize = std::rand() % 3 + 2;
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
        static_cast<float>(sz.x) / backgroundTexture.getSize().x,
        static_cast<float>(sz.y) / backgroundTexture.getSize().y
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
