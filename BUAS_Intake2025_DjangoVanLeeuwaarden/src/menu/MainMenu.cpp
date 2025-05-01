#include "MainMenu.hpp"

MainMenu::MainMenu()
    : playHovered(false), exitHovered(false),
    startGame(false), exitClicked(false) {
    playBox.setSize(sf::Vector2f(200, 50));
    playBox.setPosition(100, 100);
    playBox.setFillColor(sf::Color::Yellow);

    exitBox.setSize(sf::Vector2f(200, 50));
    exitBox.setPosition(100, 200);
    exitBox.setFillColor(sf::Color::White);
}

void MainMenu::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    // Update hover status
    playHovered = playBox.getGlobalBounds().contains(mousePos);
    exitHovered = exitBox.getGlobalBounds().contains(mousePos);

    // Update box colors based on hover
    playBox.setFillColor(playHovered ? sf::Color::Yellow : sf::Color::White);
    exitBox.setFillColor(exitHovered ? sf::Color::Yellow : sf::Color::White);

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (playHovered) {
                startGame = true;
            }
            if (exitHovered) {
                exitClicked = true;
            }
        }
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.clear();

    window.draw(playBox);
    window.draw(exitBox);

    window.display();
}

bool MainMenu::shouldSwitchState() const {
    return startGame;
}

bool MainMenu::shouldExit() const {
    return exitClicked && !startGame;
}