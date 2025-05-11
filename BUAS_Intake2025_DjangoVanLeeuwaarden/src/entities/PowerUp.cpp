#include "PowerUp.hpp"
#include <iostream>

// Define static texture storage
sf::Texture PowerUp::textures[3];

void PowerUp::initTextures()
{
    if (!textures[0].loadFromFile("assets/powerups/rapidfire.png"))
        std::cerr << "Failed to load rapidfire.png" << std::endl;
    if (!textures[1].loadFromFile("assets/powerups/multishot.png"))
        std::cerr << "Failed to load multishot.png" << std::endl;
    if (!textures[2].loadFromFile("assets/powerups/heart.png"))
        std::cerr << "Failed to load heart.png" << std::endl;
}

PowerUp::PowerUp(PowerUpType type, const sf::Vector2f& pos)
    : type(type)
{
    sprite.setTexture(textures[static_cast<int>(type)]);
    sprite.setPosition(pos);
    sprite.setScale(0.5f, 0.5f);
    auto sz = sprite.getTexture()->getSize();
    sprite.setOrigin(sz.x / 2.f, sz.y / 2.f);
}

void PowerUp::update(float dt)
{
    lifetime += dt;
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