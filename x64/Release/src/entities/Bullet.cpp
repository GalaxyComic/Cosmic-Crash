#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction) {
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(5.f, 5.f);
    shape.setPosition(position);

    velocity = direction * speed;
}

void Bullet::update(float dt) {
    shape.move(velocity * dt);
}

bool Bullet::isOffScreen(const sf::Vector2u& windowSize) const {
    sf::Vector2f pos = shape.getPosition();
    return pos.x < 0 || pos.y < 0 || pos.x > windowSize.x || pos.y > windowSize.y;
}

sf::FloatRect Bullet::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}
