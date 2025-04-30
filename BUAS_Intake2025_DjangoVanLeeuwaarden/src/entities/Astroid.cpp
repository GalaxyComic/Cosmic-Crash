#include "Astroid.hpp"

Astroid::Astroid() {
    texture.loadFromFile("assets/enemy.png");
    sprite.setTexture(texture);
    sprite.setPosition(rand() % 800, rand() % 600);
}

void Astroid::update() {
    // Update enemy state
}

void Astroid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
