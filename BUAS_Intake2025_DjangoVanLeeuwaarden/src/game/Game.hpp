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
    void updateEarthAnimation(); // Function to update Earth sprite animation

    Player player;
    std::vector<Astroid> enemies;

    sf::Texture earthTextures[60];
    sf::Sprite earthSprite;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    int currentFrame;   // Current frame in the animation
    float frameTime;    // Time interval between frames
    float elapsedTime;  // Accumulated time for animation
};
