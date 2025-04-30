#include <SFML/Graphics.hpp>
#include "../entities/Player.hpp"
#include "../entities/Astroid.hpp"

class Game {
public:
    Game();
    void run();
    void handleInput();
    void update();
    void draw();

private:
    sf::RenderWindow window;
    Player player;
    std::vector<Astroid> enemies;
};
