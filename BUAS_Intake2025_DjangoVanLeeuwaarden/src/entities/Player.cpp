#include "Player.hpp"
#include <cmath>

Player::Player() {
    texture.loadFromFile("assets/player/player_idle.png");
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    sprite.setPosition(800, 450); // center-ish
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    velocity = { 0.f, 0.f };
    window = nullptr;
}

void Player::setWindow(sf::RenderWindow* win) {
    window = win;
}

void Player::handleInput() {
    // Get the mouse position
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    sf::Vector2f playerPos = sprite.getPosition();

    // Calculate direction from player to mouse
    sf::Vector2f direction = mousePos - playerPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;  // Normalize direction
    }

    // Apply thrust if W is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float acceleration = 150.f; // Acceleration when thrusting
        velocity += direction * acceleration * 0.016f;  // Add velocity based on direction and time
    }
}

void Player::update() {
    if (!window) return;

    // Apply velocity
    sprite.move(velocity * 0.016f);

    // Slight drag to avoid infinite speed
    float drag = 0.995f;
    velocity *= drag;

    // Rotate to face mouse
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    sf::Vector2f playerPos = sprite.getPosition();

    sf::Vector2f direction = mousePos - playerPos;
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f); // Adjust if your sprite points up
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
