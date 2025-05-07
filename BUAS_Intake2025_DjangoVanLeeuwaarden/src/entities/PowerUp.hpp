#pragma once
#include <SFML/Graphics.hpp>

enum class PowerUpType
{
    RapidFire,
    MultiShot,
    GainLife,
    Laser
};

class PowerUp : public sf::Drawable {
public:
    static void initTextures();
    PowerUp(PowerUpType type, const sf::Vector2f& pos);
    void update(float dt);
    sf::FloatRect getGlobalBounds() const;
    PowerUpType getType() const;

    bool operator==(const PowerUp& other) const {
        return this->type == other.type;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite sprite;
    sf::Texture texture;
    PowerUpType type;
    float lifetime = 0.f;
};
