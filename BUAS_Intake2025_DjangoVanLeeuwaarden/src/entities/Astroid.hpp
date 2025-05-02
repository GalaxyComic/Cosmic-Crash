#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>

class Astroid : public sf::Drawable {
public:
    // Construct with the Earth's position (target) and the window size
    Astroid(const sf::Vector2f& targetPos, const sf::Vector2u& windowSize);

    // Move each frame (with real delta-time)
    void update(float dt);

    // For collision detection
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

private:
    // sf::Drawable override
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(sprite, states);
    }

    sf::Sprite            sprite;
    static sf::Texture    texture;        // shared texture for all asteroids
    sf::Vector2f          velocity;       // direction * speed
    float                 speed = 200.f;  // pixels per second
};