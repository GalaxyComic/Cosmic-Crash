#include "MainMenu.hpp"

MainMenu::MainMenu()
    : playSelected(true), exitSelected(false), startGame(false) {

    playBox.setSize(sf::Vector2f(200, 50));
    playBox.setPosition(100, 100);
    playBox.setFillColor(sf::Color::Yellow);

    exitBox.setSize(sf::Vector2f(200, 50));
    exitBox.setPosition(100, 200);
    exitBox.setFillColor(sf::Color::White);
}

void MainMenu::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            exitSelected = true;
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                playSelected = !playSelected;
                exitSelected = !exitSelected;
            }
            if (event.key.code == sf::Keyboard::Enter) {
                if (playSelected) {
                    startGame = true;
                }
                else if (exitSelected) {
                    exitSelected = true;
                }
            }
        }
    }

    // Update colors based on selection
    playBox.setFillColor(playSelected ? sf::Color::Yellow : sf::Color::White);
    exitBox.setFillColor(exitSelected ? sf::Color::Yellow : sf::Color::White);
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
    return exitSelected && !startGame;
}
