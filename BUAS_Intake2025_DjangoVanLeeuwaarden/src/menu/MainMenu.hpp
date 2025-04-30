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
    sf::RectangleShape playBox;
    sf::RectangleShape exitBox;

    bool playSelected;
    bool exitSelected;
    bool startGame;
};
