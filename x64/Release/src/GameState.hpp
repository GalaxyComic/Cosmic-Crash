#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual bool shouldExit() const { return false; }
    virtual bool shouldSwitchState() const { return false; }
};