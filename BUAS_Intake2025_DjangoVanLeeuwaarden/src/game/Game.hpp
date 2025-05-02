// Game.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../entities/Player.hpp"
#include "../entities/Astroid.hpp"
#include "../entities/Bullet.hpp"
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
    void updateEarthAnimation(float dt);
    void centerEarthSprite(const sf::RenderWindow& window);
    void spawnAstroid();

    Player                          player;
    std::vector<Astroid>            enemies;

    // Earth animation
    static constexpr int            EARTH_FRAMES = 60;
    sf::Texture                     earthTextures[EARTH_FRAMES];
    sf::Sprite                      earthSprite;

    // Background
    sf::Texture                     backgroundTexture;
    sf::Sprite                      backgroundSprite;

    // Game state
    int                             lives = 3;
    
    // Shooting
    std::vector<Bullet> bullets;
    sf::Clock shootCooldown;
    
    // Timing
    sf::Clock                       clock;                  // measures real elapsed time
    int                             currentFrame = 0;
    float                           frameTime = 1.f / 5.f;  // seconds per Earth frame (5 FPS)
    float                           elapsedTime = 0.f;      // accumulates dt for Earth
    float                           spawnTimer = 0.f;       // accumulates dt for spawning

    bool                            backToMainMenu = false;
    sf::RenderWindow* windowPtr = nullptr;
};
