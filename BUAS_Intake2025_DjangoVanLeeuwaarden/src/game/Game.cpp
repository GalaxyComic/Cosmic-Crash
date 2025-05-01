#include "Game.hpp"
#include <iostream>
#include <algorithm>

Game::Game()
    : currentFrame(0)
    , frameTime(15.f)
    , elapsedTime(0.f)
    , backToMainMenu(false)
    , windowPtr(nullptr)
{
    // Background
    if (!backgroundTexture.loadFromFile("assets/background.png"))
        std::cerr << "Error loading background texture!\n";
    backgroundSprite.setTexture(backgroundTexture);

    // Load Earth frames
    for (int i = 0; i < 60; i++) {
        if (!earthTextures[i].loadFromFile(
            "assets/earth/4169310314-" + std::to_string(i) + ".png"))
        {
            std::cerr << "Error loading Earth frame " << i << "!\n";
        }
    }
    earthSprite.setTexture(earthTextures[0]);
    // Center the origin so positioning and collision use the true center
    auto eb = earthSprite.getLocalBounds();
    earthSprite.setOrigin(eb.width / 2.f, eb.height / 2.f);
}

void Game::handleInput(sf::RenderWindow& window) {
    windowPtr = &window;

    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();

        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Escape)
        {
            backToMainMenu = true;
        }
    }

    player.setWindow(&window);
    player.handleInput();
}

void Game::centerEarthSprite(const sf::RenderWindow& window) {
    earthSprite.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f
    );
}

void Game::scaleBackgroundToFit(const sf::RenderWindow& window) {
    auto sz = window.getSize();
    backgroundSprite.setScale(
        float(sz.x) / backgroundTexture.getSize().x,
        float(sz.y) / backgroundTexture.getSize().y
    );
}

void Game::updateEarthAnimation() {
    elapsedTime += 1.f / 60.f;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % 60;
        earthSprite.setTexture(earthTextures[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Game::spawnAstroid() {
    if (windowPtr && lives > 0) {
        enemies.emplace_back(
            earthSprite.getPosition(),
            windowPtr->getSize()
        );
    }
}

void Game::update() {
    // 1) Re-center Earth each frame
    if (windowPtr)
        centerEarthSprite(*windowPtr);

    // 2) Update player and asteroids
    player.update();
    for (auto& a : enemies)
        a.update();

    // 3) Spawn new asteroid every 2 seconds
    static float spawnTimer = 0.f;
    spawnTimer += 1.f / 60.f;
    if (spawnTimer >= 2.f) {
        spawnAstroid();
        spawnTimer = 0.f;
    }

    // 4) Collision: Asteroids vs. Earth sprite
    sf::FloatRect earthBounds = earthSprite.getGlobalBounds();
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [&](Astroid& a) {
                if (a.getGlobalBounds().intersects(earthBounds)) {
                    lives--;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
    if (lives < 0) lives = 0;

    // 5) Advance Earth animation
    updateEarthAnimation();
}

void Game::draw(sf::RenderWindow& window) {
    window.clear();

    // Draw background
    scaleBackgroundToFit(window);
    window.draw(backgroundSprite);

    // Draw Earth (scaled up)
    earthSprite.setScale(2.f, 2.f);
    window.draw(earthSprite);

    // Draw player
    window.draw(player);

    // Draw all asteroids
    for (auto& a : enemies)
        window.draw(a);

    // Draw lives
    static sf::Font font;
    if (!font.getInfo().family.size()) {
        if (!font.loadFromFile("assets/arial.ttf"))
            std::cerr << "Failed to load font\n";
    }
    sf::Text livesText("Lives: " + std::to_string(lives), font, 24);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10.f, 10.f);
    window.draw(livesText);

    window.display();
}
