#include "Astroid.hpp"
#include <cmath>
#include <cstdlib>

// Define the static texture
sf::Texture Astroid::texture;

Astroid::Astroid(const sf::Vector2f& targetPos, const sf::Vector2u& windowSize) {
    // Lazy-load the texture once
    if (texture.getSize().x == 0) {
        if (!texture.loadFromFile("assets/enemy.png")) {
            std::cerr << "Failed to load assets/enemy.png\n";
        }
    }

    // Set up the sprite
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);

    // Center origin
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Spawn at random window edge
    int side = std::rand() % 4;
    float x, y;
    switch (side) {
    case 0: // top
        x = static_cast<float>(std::rand() % windowSize.x);
        y = 0.f;
        break;
    case 1: // right
        x = static_cast<float>(windowSize.x);
        y = static_cast<float>(std::rand() % windowSize.y);
        break;
    case 2: // bottom
        x = static_cast<float>(std::rand() % windowSize.x);
        y = static_cast<float>(windowSize.y);
        break;
    default: // left
        x = 0.f;
        y = static_cast<float>(std::rand() % windowSize.y);
        break;
    }
    sprite.setPosition(x, y);

    // Compute normalized velocity toward target
    sf::Vector2f dir = targetPos - sprite.getPosition();
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0.f) {
        velocity = (dir / length) * speed;
    }
    else {
        velocity = sf::Vector2f(0.f, 0.f);
    }
}

void Astroid::update(float dt) {
    // Move by velocity and delta-time
    sprite.move(velocity * dt);
}
