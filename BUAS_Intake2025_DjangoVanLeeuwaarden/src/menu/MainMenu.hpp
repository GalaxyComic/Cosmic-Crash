#pragma once
#include <SFML/Graphics.hpp>
#include "../GameState.hpp"

class MainMenu : public GameState {
public:
    MainMenu();

    void handleInput(sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    bool shouldSwitchState() const override;
    bool shouldExit() const override;

private:
    sf::Texture playTexture;
    sf::Sprite  playSprite;
    sf::Vector2f playOriginalPos;

    sf::Texture exitTexture;
    sf::Sprite  exitSprite;
    sf::Vector2f exitOriginalPos;

    bool playHovered;
    bool exitHovered;
    bool startGame;
    bool exitClicked;

    sf::Clock animationClock;

    void scaleBackgroundToFit(const sf::RenderWindow& window);
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};
