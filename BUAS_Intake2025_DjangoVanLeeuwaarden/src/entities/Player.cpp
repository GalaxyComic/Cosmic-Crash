#include "Player.hpp"

Player::Player() {
    texture.loadFromFile("assets/player.png");
    sprite.setTexture(texture);
    sprite.setPosition(400, 300);
}

void Player::handleInput() {
    // Handle player input
}

void Player::update() {
    // Update player state
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
