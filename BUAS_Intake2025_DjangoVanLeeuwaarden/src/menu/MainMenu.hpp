#pragma once
#include <SFML/Graphics.hpp>
#include "../GameState.hpp"

class MainMenu : public GameState {
public:
    MainMenu();

    void handleInput(sf::RenderWindow& window) override;
    void update() override {}
    void draw(sf::RenderWindow& window) override;

    bool shouldSwitchState() const override;
    bool shouldExit() const override;

private:

    // Menu Buttons
    sf::RectangleShape playBox;
    sf::RectangleShape exitBox;
    bool            playHovered;
    bool            exitHovered;
    bool            startGame;
    bool            exitClicked;

    // Background
    void scaleBackgroundToFit(const sf::RenderWindow& window);
    sf::Texture     backgroundTexture;
    sf::Sprite      backgroundSprite;
};
