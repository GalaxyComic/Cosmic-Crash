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

	void centerEarthSprite(const sf::RenderWindow& window); // Function to center the Earth sprite

    bool goBackToMainMenu() const { return backToMainMenu; } // Getter for backToMainMenu

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

	bool backToMainMenu = false; // Flag to switch back to main menu
};
