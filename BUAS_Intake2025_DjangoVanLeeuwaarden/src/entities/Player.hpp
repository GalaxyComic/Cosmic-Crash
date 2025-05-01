#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
public:
    Player();
    void handleInput();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setWindow(sf::RenderWindow* win);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    sf::RenderWindow* window;
};
