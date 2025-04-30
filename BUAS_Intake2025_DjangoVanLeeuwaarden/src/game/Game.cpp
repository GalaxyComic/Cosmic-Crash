#include "Game.hpp"

Game::Game() {
    enemies.emplace_back();
}

void Game::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    player.handleInput();
}

void Game::update() {
    player.update();
    for (auto& enemy : enemies)
        enemy.update();
}

void Game::draw(sf::RenderWindow& window) {
    window.clear();
    window.draw(player);
    for (const auto& enemy : enemies)
        window.draw(enemy);
    window.display();
}
