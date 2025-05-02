#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable {
public:
    Bullet(sf::Vector2f position, sf::Vector2f direction);

    void update(float dt);
    bool isOffScreen(const sf::Vector2u& windowSize) const;
    sf::FloatRect getGlobalBounds() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed = 500.f;
};