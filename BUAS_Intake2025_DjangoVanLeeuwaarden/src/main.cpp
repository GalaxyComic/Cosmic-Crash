#include <SFML/Graphics.hpp>
#include "menu/MainMenu.hpp"
#include "game/Game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game");

    MainMenu menu;
    Game game;

    bool inMenu = true;

    while (window.isOpen()) {
        if (inMenu) {
            menu.handleInput(window);
            if (menu.isPlaySelected()) {
                inMenu = false;
                game.run();
            }
            if (menu.isExitSelected()) {
                window.close();
            }
            menu.draw(window);
        }
    }

    return 0;
}
