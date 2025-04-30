#include <SFML/Graphics.hpp>

class MainMenu {
public:
    MainMenu();
    void handleInput(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool isPlaySelected() const;
    bool isExitSelected() const;

private:
    sf::Font font;
    sf::Text playText;
    sf::Text exitText;
    bool playSelected;
    bool exitSelected;
};
