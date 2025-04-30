#include <SFML/Graphics.hpp>

int main()
{
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Orbital Overdrive");
    window.setFramerateLimit(60);

    // Load the pixel art texture
    sf::Texture earthTexture;
    if (!earthTexture.loadFromFile("assets/earth_pixel_art.png")) {
        // Handle loading error
        return -1;
    }

    // Disable smoothing to preserve pixelated look
    earthTexture.setSmooth(false);

    // Create a sprite for Earth
    sf::Sprite earthSprite;
    earthSprite.setTexture(earthTexture);

    // Set the origin to the center of the sprite
    sf::Vector2u textureSize = earthTexture.getSize();
    earthSprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

    // Position the sprite at the center of the window
    earthSprite.setPosition(windowWidth / 2.f, windowHeight / 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(earthSprite);
        window.display();
    }

    return 0;
}
