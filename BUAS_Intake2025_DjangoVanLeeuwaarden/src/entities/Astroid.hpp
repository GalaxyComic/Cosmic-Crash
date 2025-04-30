#pragma once
#include <SFML/Graphics.hpp>

class Astroid : public sf::Drawable {
public:
    Astroid();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Sprite sprite;
    sf::Texture texture;
};
