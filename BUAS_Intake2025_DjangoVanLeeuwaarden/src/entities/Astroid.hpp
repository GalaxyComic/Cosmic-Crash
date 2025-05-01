#pragma once
#include <SFML/Graphics.hpp>

class Astroid : public sf::Drawable {
public:
    // Must provide the target position (earth center) and window size
    Astroid(const sf::Vector2f& targetPos, const sf::Vector2u& windowSize);

    // Move each frame
    void update();

    // Draw override
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // For collision detection
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

private:
    sf::Sprite    sprite;
    sf::Texture   texture;  
	sf::Vector2f  velocity;  // normalized direction * speed
    float         speed = 200.f;  // pixels per second
};
