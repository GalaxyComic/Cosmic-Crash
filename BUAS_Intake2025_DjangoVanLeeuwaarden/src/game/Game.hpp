#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../entities/Player.hpp"
#include "../entities/Astroid.hpp"
#include "../entities/Bullet.hpp"
#include "../entities/PowerUp.hpp"
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
    void spawnAstroidCluster();
    void spawnPowerUp();

    // Player
    Player                          player;
    std::vector<Astroid>            enemies;
    std::vector<PowerUp>            powerUps;

    // Earth animation
    static constexpr int            EARTH_FRAMES = 60;
    sf::Texture                     earthTextures[EARTH_FRAMES];
    sf::Sprite                      earthSprite;

    // Background
    sf::Texture                     backgroundTexture;
    sf::Sprite                      backgroundSprite;

    // Game state
    int                             lives = 3;
    sf::Texture heartTexture;
    sf::Sprite heartSprite;
    bool gameOver = false;
    sf::Text gameOverText;
    sf::Text finalScoreText;

    // Score
    int score = 0;
    sf::Font font;
    sf::Text scoreText;

    // Shooting
    std::vector<Bullet> bullets;
    sf::Clock shootCooldown;

    // Timing
    sf::Clock                       clock;                  // measures real elapsed time
    int                             currentFrame = 0;
    float                           frameTime = 1.f / 5.f;  // seconds per Earth frame (5 FPS)
    float                           elapsedTime = 0.f;      // accumulates dt for Earth
    float                           spawnTimer = 0.f;       // accumulates dt for spawning

    // Powerups
    bool                            multiShotActive = false;
    sf::Clock                       multiShotTimer;
    bool                            rapidFireActive = false;
    sf::Clock                       rapidFireTimer;
    bool                            laserActive = false;    // Add laser power-up flag

    bool                            backToMainMenu = false;
    sf::RenderWindow* windowPtr = nullptr;

    // Power-up spawn
    sf::Clock spawnPowerUpClock;
    float spawnPowerUpInterval = 10.f; // Power-up spawn interval
};
