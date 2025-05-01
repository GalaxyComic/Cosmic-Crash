#include <SFML/Graphics.hpp>
#include "menu/MainMenu.hpp"
#include "game/Game.hpp"

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Game", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    GameState* state = new MainMenu();

    while (window.isOpen()) {
        state->handleInput(window);
        state->update();
        state->draw(window);

        if (state->shouldExit()) {
            window.close();
        }
        else if (state->shouldSwitchState()) {
            delete state;
            state = new Game();
        }
        
        else if (dynamic_cast<Game*>(state) && static_cast<Game*>(state)->goBackToMainMenu()) {
            delete state;
            state = new MainMenu();
        }
    }

    delete state;
    return 0;
}

