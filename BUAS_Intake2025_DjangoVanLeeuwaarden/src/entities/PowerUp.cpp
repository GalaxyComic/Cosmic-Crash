#include "PowerUp.hpp"
#include <iostream>

PowerUp::PowerUp(PowerUpType type, const sf::Vector2f& pos)
    : type(type)
{
    // Choose texture based on power-up type
    std::string path;
    switch (type) {
    case PowerUpType::RapidFire: path = "assets/powerups/rapidfire.png"; break;
    case PowerUpType::MultiShot: path = "assets/powerups/multishot.png"; break;
    case PowerUpType::GainLife:  path = "assets/powerups/heart.png"; break;
    case PowerUpType::Laser:     path = "assets/powerups/laser.png"; break;
    }
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load power-up texture: " << path << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(0.5f, 0.5f);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

void PowerUp::update(float dt)
{
    lifetime += dt;
    // Optionally: add some animation (e.g., floating effect) here
}

sf::FloatRect PowerUp::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

PowerUpType PowerUp::getType() const
{
    return type;
}

void PowerUp::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}