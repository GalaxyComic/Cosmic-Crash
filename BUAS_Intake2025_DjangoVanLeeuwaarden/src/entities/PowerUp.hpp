#pragma once

#include <SFML/Graphics.hpp>

enum class PowerUpType { 
    RapidFire, 
    MultiShot, 
    GainLife, 
};

class PowerUp : public sf::Drawable {
public:
    // Call once at startup to load all power-up textures
    static void initTextures();

    PowerUp(PowerUpType type, const sf::Vector2f& pos);
    void update(float dt);
    sf::FloatRect getGlobalBounds() const;
    PowerUpType getType() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    static sf::Texture textures[3];
    sf::Sprite sprite;
    PowerUpType type;
    float lifetime = 0.f;
};
