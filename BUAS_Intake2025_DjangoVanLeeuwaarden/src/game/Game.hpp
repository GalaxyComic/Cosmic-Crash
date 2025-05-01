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

    bool goBackToMainMenu() const { return backToMainMenu; }

private:
    void scaleBackgroundToFit(const sf::RenderWindow& window);
    void updateEarthAnimation();
    void centerEarthSprite(const sf::RenderWindow& window);
    void spawnAstroid();

    Player                  player;
    std::vector<Astroid>    enemies;

    // Earth animation
    sf::Texture             earthTextures[60];
    sf::Sprite              earthSprite;

    // Background
    sf::Texture             backgroundTexture;
    sf::Sprite              backgroundSprite;

    // Game state
    int                     lives = 3;

    // Animation timing
    int                     currentFrame;
    float                   frameTime;
    float                   elapsedTime;

    bool                    backToMainMenu = false;
    sf::RenderWindow* windowPtr = nullptr;
};
