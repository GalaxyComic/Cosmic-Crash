#pragma once
#include <SFML/Graphics.hpp>
#include "../entities/Player.hpp"
#include "../entities/Astroid.hpp"
#include "../GameState.hpp"
#include <vector>

class Game : public GameState {
public:
    Game();

    void handleInput(sf::RenderWindow& window) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    void centerEarthSprite(const sf::RenderWindow& window);

private:
    void scaleBackgroundToFit(const sf::RenderWindow& window);

    Player player;
    std::vector<Astroid> enemies;

    sf::Texture earthTexture;
    sf::Sprite earthSprite;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};
