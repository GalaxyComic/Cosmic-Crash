#include "MainMenu.hpp"

MainMenu::MainMenu()
    : playSelected(false), exitSelected(false) {
    font.loadFromFile("assets/font.ttf");
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(24);
    playText.setPosition(100, 100);

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(24);
    exitText.setPosition(100, 150);
}

void MainMenu::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                playSelected = !playSelected;
                exitSelected = !exitSelected;
            }
            if (event.key.code == sf::Keyboard::Enter) {
                if (playSelected)
					playSelected = true;
                else if (exitSelected)
                    window.close();
            }
        }
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(playText);
    window.draw(exitText);
}

bool MainMenu::isPlaySelected() const {
    return playSelected;
}

bool MainMenu::isExitSelected() const {
    return exitSelected;
}
