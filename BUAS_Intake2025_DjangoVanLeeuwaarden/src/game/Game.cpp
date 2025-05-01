#include "Game.hpp"
#include <iostream>

Game::Game()
    : currentFrame(0), frameTime(15.0f), elapsedTime(0.f) {

    // Load the background texture
    if (!backgroundTexture.loadFromFile("assets/background.png")) {
        std::cerr << "Error loading background texture!" << std::endl;
    }

    // Load each frame of the Earth GIF (120frames)
    for (int i = 0; i < 60; i++) {
        if (!earthTextures[i].loadFromFile("assets/earth/4169310314-" + std::to_string(i) + ".png")) {
            std::cerr << "Error loading Earth frame " << i << "!" << std::endl;
        }
    }

    // Set textures to sprites
    backgroundSprite.setTexture(backgroundTexture);
    earthSprite.setTexture(earthTextures[currentFrame]);

    // Ensure the Earth sprite is centered by setting the origin
    sf::FloatRect bounds = earthSprite.getLocalBounds();
    earthSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Set initial position of Earth to the center of the window
    earthSprite.setPosition(400.f, 300.f);
}

void Game::centerEarthSprite(const sf::RenderWindow& window) {
    // Center the Earth sprite
    earthSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

void Game::scaleBackgroundToFit(const sf::RenderWindow& window) {
    // Get the window size and background texture size
    sf::Vector2u windowSize = window.getSize();
    float scaleX = float(windowSize.x) / float(backgroundTexture.getSize().x);
    float scaleY = float(windowSize.y) / float(backgroundTexture.getSize().y);

    // Scale the background sprite to fit the window
    backgroundSprite.setScale(scaleX, scaleY);
}

void Game::updateEarthAnimation() {
    // Update the animation frame based on elapsed time
    elapsedTime += 1.f / 60.f;  // Assuming a 60 FPS game loop

    if (elapsedTime >= frameTime) {
        // Increment current frame and loop if necessary
        currentFrame = (currentFrame + 1) % 60;  // Assuming 60 frames

        // Update the Earth sprite texture to the current frame
        earthSprite.setTexture(earthTextures[currentFrame]);

        // Reset the timer for the next frame
        elapsedTime = 0.f;
    }
}

void Game::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    // Set window pointer for the player to access mouse position
    player.setWindow(&window);

    // Handle player movement input
    player.handleInput();
}


void Game::update() {
    // Update the player and enemies
    player.update();
    for (auto& enemy : enemies) {
        enemy.update();
    }

    // Update Earth animation
    updateEarthAnimation();
}

void Game::draw(sf::RenderWindow& window) {
    window.clear();

    // Scale the background to fit the window size
    scaleBackgroundToFit(window);

    // Center the Earth sprite
    centerEarthSprite(window);
    earthSprite.setScale(2.f, 2.f);

    // Draw background, Earth, player, and enemies
    window.draw(backgroundSprite);
    window.draw(earthSprite);
    window.draw(player);

    for (const auto& enemy : enemies) {
        window.draw(enemy);
    }

    window.display();
}
