#include "Game.hpp"

Game::Game() {
    // Load textures
    backgroundTexture.loadFromFile("assets/background.png");
    earthTexture.loadFromFile("assets/earth_pixel_art.png");

    // Set textures to sprites
    backgroundSprite.setTexture(backgroundTexture);
    earthSprite.setTexture(earthTexture);

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

void Game::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    // Handle player input (from the player class)
    player.handleInput();
}

void Game::update() {
    // Update the player and enemies
    player.update();
    for (auto& enemy : enemies) {
        enemy.update();
    }
}

void Game::draw(sf::RenderWindow& window) {
    window.clear();

    // Scale the background to fit the window size
    scaleBackgroundToFit(window);

	// Center the Earth sprite
	centerEarthSprite(window);

    // Draw background, Earth, player, and enemies
    window.draw(backgroundSprite);
    window.draw(earthSprite);
    window.draw(player);

    for (const auto& enemy : enemies) {
        window.draw(enemy);
    }

    window.display();
}
