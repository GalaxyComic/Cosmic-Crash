#include "MainMenu.hpp"
#include <iostream>
#include <cmath>

MainMenu::MainMenu()
    : playHovered(false), exitHovered(false),
    startGame(false), exitClicked(false) {

    if (!playTexture.loadFromFile("assets/ui/play.png"))
        std::cerr << "Error loading play button image!\n";
    playSprite.setTexture(playTexture);
    playSprite.setPosition(100, 200);
    playOriginalPos = playSprite.getPosition();

    if (!exitTexture.loadFromFile("assets/ui/exit.png"))
        std::cerr << "Error loading exit button image!\n";
    exitSprite.setTexture(exitTexture);
    exitSprite.setPosition(100, 600);
    exitOriginalPos = exitSprite.getPosition();

    if (!backgroundTexture.loadFromFile("assets/ui/background.png"))
        std::cerr << "Error loading background texture!\n";
    backgroundSprite.setTexture(backgroundTexture);
}

void MainMenu::handleInput(sf::RenderWindow& window) {
    sf::Event event;
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    playHovered = playSprite.getGlobalBounds().contains(mousePos);
    exitHovered = exitSprite.getGlobalBounds().contains(mousePos);

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

void MainMenu::scaleBackgroundToFit(const sf::RenderWindow& window) {
    auto sz = window.getSize();
    backgroundSprite.setScale(
        float(sz.x) / backgroundTexture.getSize().x,
        float(sz.y) / backgroundTexture.getSize().y
    );
}

void MainMenu::update() {
    float time = animationClock.getElapsedTime().asSeconds();
    float shakeOffset = std::sin(time * 10.f) * 2.f; // fast small shake

    if (playHovered) {
        playSprite.setScale(1.1f, 1.1f);
        playSprite.setPosition(playOriginalPos.x + shakeOffset, playOriginalPos.y);
    }
    else {
        playSprite.setScale(1.f, 1.f);
        playSprite.setPosition(playOriginalPos);
    }

    if (exitHovered) {
        exitSprite.setScale(1.1f, 1.1f);
        exitSprite.setPosition(exitOriginalPos.x + shakeOffset, exitOriginalPos.y);
    }
    else {
        exitSprite.setScale(1.f, 1.f);
        exitSprite.setPosition(exitOriginalPos);
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.clear();

    scaleBackgroundToFit(window);
    window.draw(backgroundSprite);

    window.draw(playSprite);
    window.draw(exitSprite);

    window.display();
}

bool MainMenu::shouldSwitchState() const {
    return startGame;
}

bool MainMenu::shouldExit() const {
    return exitClicked && !startGame;
}