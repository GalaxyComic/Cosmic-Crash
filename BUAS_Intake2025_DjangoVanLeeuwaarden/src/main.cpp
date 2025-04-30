#include <SFML/Graphics.hpp>
#include "menu/MainMenu.hpp"
#include "game/Game.hpp"

int main() {
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktop, "Game", sf::Style::Fullscreen);

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
    }

    delete state;
    return 0;
}
