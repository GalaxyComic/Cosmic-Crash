#include "Game.hpp"

Game::Game()
    : window(sf::VideoMode(800, 600), "Game") {
    // Initialisatiecode hier
}

void Game::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        draw();
    }
}

void Game::handleInput() {
    // Verwerk gebruikersinvoer
}

void Game::update() {
    // Werk de spelstatus bij
}

void Game::draw() {
    window.clear();
    window.draw(player);
    for (const auto& astroid : enemies)
        window.draw(astroid);
    window.display();
}
