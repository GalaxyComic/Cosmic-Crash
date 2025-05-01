#include "Player.hpp"
#include <cmath>

Player::Player() {
    texture.loadFromFile("assets/player.png");
    sprite.setTexture(texture);
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

    // Calculate the direction vector from player to mouse
    sf::Vector2f direction = mousePos - playerPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;  // Normalize the direction vector
    }

    // Move the spaceship in the direction of the mouse when "W" is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float speed = 200.f;  // Set your speed
        sprite.move(direction * speed * 0.016f); // Move in direction (scaled by delta time)
    }
}

void Player::update() {
    // Get the mouse position relative to the window
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    sf::Vector2f playerPos = sprite.getPosition();

    // Calculate the direction vector from player to mouse
    sf::Vector2f direction = mousePos - playerPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize the direction vector to avoid movement speed issues
    if (length != 0) {
        direction /= length;
    }

    // Calculate the angle to rotate the spaceship
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;

    // Set the sprite's rotation to face the mouse cursor
    sprite.setRotation(angle + 90.f); // +90 if the sprite points up

    // Handle movement (move when W is pressed)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float speed = 200.f;  // Speed of the spaceship
        sprite.move(direction * speed * 0.016f); // Move towards the mouse direction (scaled by delta time)
    }
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
