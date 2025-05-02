#include "Game.hpp"
#include <iostream>
#include <algorithm>

Game::Game()
{
    // Background
    if (!backgroundTexture.loadFromFile("assets/background.png"))
        std::cerr << "Error loading background texture!\n";
    backgroundSprite.setTexture(backgroundTexture);

    // Load Earth frames
    for (int i = 0; i < EARTH_FRAMES; i++) {
        const std::string path = "assets/earth/4169310314-" + std::to_string(i) + ".png";
        if (!earthTextures[i].loadFromFile(path)) {
            std::cerr << "Error loading Earth frame " << i << " (" << path << ")!\n";
        }
    }
    earthSprite.setTexture(earthTextures[0]);
    // Center origin for rotation/positioning
    auto eb = earthSprite.getLocalBounds();
    earthSprite.setOrigin(eb.width / 2.f, eb.height / 2.f);
}

void Game::handleInput(sf::RenderWindow& window)
{
    windowPtr = &window;
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();
        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Escape)
        {
            backToMainMenu = true; // go back to menu / game-over
        }
    }

    player.setWindow(&window);
    player.handleInput();
}

void Game::update()  
{  
   if (!windowPtr) return;  

   // 1) Compute real delta‑time  
   float dt = clock.restart().asSeconds();  

   // 2) Re-center Earth each frame  
   centerEarthSprite(*windowPtr);  

   // 3) Earth animation  
   updateEarthAnimation(dt);  

   // 4) Update player & asteroids  
   player.update();  
   for (auto& a : enemies)  
       a.update(dt);  

   // 5) Spawn new asteroid every 2 seconds  
   spawnTimer += dt;  
   if (spawnTimer >= 2.f) {  
       spawnAstroid();  
       spawnTimer -= 2.f;  
   }  

   // 6) Check collisions (asteroid vs Earth)  
   sf::FloatRect earthBounds = earthSprite.getGlobalBounds();  
   enemies.erase(  
       std::remove_if(enemies.begin(), enemies.end(),  
           [&](Astroid& a) {  
               if (a.getGlobalBounds().intersects(earthBounds)) {  
                   --lives;  
                   return true;  
               }  
               return false;  
           }),  
       enemies.end()  
   );  
   if (lives <= 0) {  
       lives = 0;  
       backToMainMenu = true;  
   }  
}

void Game::draw(sf::RenderWindow& window)
{
    window.clear();

    // Background
    scaleBackgroundToFit(window);
    window.draw(backgroundSprite);

    // Draw Earth (scaled up)
    earthSprite.setScale(2.f, 2.f);
    window.draw(earthSprite);

    // Player & asteroids
    window.draw(player);
    for (auto& a : enemies)
        window.draw(a);

    // TODO: draw HUD / lives here

    window.display();
}

void Game::centerEarthSprite(const sf::RenderWindow& window)
{
    earthSprite.setPosition(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f
    );
}

void Game::scaleBackgroundToFit(const sf::RenderWindow& window)
{
    auto sz = window.getSize();
    backgroundSprite.setScale(
        float(sz.x) / backgroundTexture.getSize().x,
        float(sz.y) / backgroundTexture.getSize().y
    );
}

void Game::updateEarthAnimation(float dt)
{
    elapsedTime += dt;
    if (elapsedTime >= frameTime) {
        currentFrame = (currentFrame + 1) % EARTH_FRAMES;
        earthSprite.setTexture(earthTextures[currentFrame]);
        elapsedTime -= frameTime;  // preserve any leftover
    }
}

void Game::spawnAstroid()
{
    if (windowPtr && lives > 0) {
        enemies.emplace_back(
            earthSprite.getPosition(),
            windowPtr->getSize()
        );
    }
}
