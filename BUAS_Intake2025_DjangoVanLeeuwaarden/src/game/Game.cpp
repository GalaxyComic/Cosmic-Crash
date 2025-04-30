#include "Game.hpp"

Game::Game()
    : window(sf::VideoMode(800, 600), "Game") {
    // Initialize game elements
}

void Game::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        draw();
    }
}

void Game::handleInput() {
    // Handle player input
}

void Game::update() {
    // Update game logic
}

void Game::draw() {
    window.clear();
    window.draw(player);
    for (const auto& astroid : enemies)
        window.draw(astroid);
    window.display();
}
