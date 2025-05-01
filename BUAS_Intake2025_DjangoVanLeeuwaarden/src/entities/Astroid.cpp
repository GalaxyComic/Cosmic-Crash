#include "Astroid.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

Astroid::Astroid(const sf::Vector2f& targetPos, const sf::Vector2u& windowSize) {
    // Load the texture
    if (!texture.loadFromFile("assets/enemy.png")) {
        std::cerr << "Failed to load assets/enemy.png\n";
    }
	sprite.setScale(0.1f, 0.1f); // Scale the sprite to half its size
    sprite.setTexture(texture);

    // Center the sprite origin so setPosition uses its center
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Spawn at a random window edge
    int side = rand() % 4;
    float x, y;
    switch (side) {
    case 0: // top
        x = static_cast<float>(rand() % windowSize.x);
        y = 0.f;
        break;
    case 1: // right
        x = static_cast<float>(windowSize.x);
        y = static_cast<float>(rand() % windowSize.y);
        break;
    case 2: // bottom
        x = static_cast<float>(rand() % windowSize.x);
        y = static_cast<float>(windowSize.y);
        break;
    default: // left
        x = 0.f;
        y = static_cast<float>(rand() % windowSize.y);
        break;
    }
    sprite.setPosition(x, y);

    // Compute normalized velocity toward the target position
    sf::Vector2f dir = targetPos - sprite.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0.f) {
        velocity = (dir / length) * speed;
    }
    else {
        velocity = { 0.f, 0.f };
    }
}

void Astroid::update() {
    // Move by velocity scaled to a fixed 60 FPS timestep
    sprite.move(velocity * (1.f / 60.f));
}

void Astroid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
