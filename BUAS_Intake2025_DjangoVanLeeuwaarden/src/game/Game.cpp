#include "Game.hpp"
#include <iostream>
#include <algorithm>

Game::Game()
{
    // Lives
    if (!heartTexture.loadFromFile("assets/ui/heart.png"))
        std::cerr << "Error loading heart texture!\n";
    heartSprite.setScale(2.5, 2.5);
    heartSprite.setTexture(heartTexture);
    
    // Background
    if (!backgroundTexture.loadFromFile("assets/ui/background.png"))
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

        sf::Vector2f direction;
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            if (shootCooldown.getElapsedTime().asSeconds() > 0.25f) {
                if (player.shootDirection(direction)) {
                    bullets.emplace_back(player.getPosition(), direction);
                    shootCooldown.restart();
                }
            }
        }
    }

    player.setWindow(&window);
    player.handleInput();
}

void Game::update()  
{  
   if (!windowPtr) return;  

   // Compute real delta‑time  
   float dt = clock.restart().asSeconds();  

   // Re-center Earth each frame  
   centerEarthSprite(*windowPtr);  

   // Earth animation  
   updateEarthAnimation(dt);  

   // Update player & asteroids  
   player.update();  
   for (auto& a : enemies)  
       a.update(dt);  

   // Update bullets
   for (auto& b : bullets)
       b.update(dt);

   // Spawn new asteroid every 2 seconds  
   spawnTimer += dt;  
   if (spawnTimer >= 2.f) {  
       spawnAstroid();  
       spawnTimer -= 2.f;  
   }  
   // Check collisions (Bullets vs astroid
   for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
       bool bulletRemoved = false;

       for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
           if (bulletIt->getGlobalBounds().intersects(enemyIt->getGlobalBounds())) {
               // Remove both bullet and asteroid on hit
               enemyIt = enemies.erase(enemyIt);
               bulletIt = bullets.erase(bulletIt);
               bulletRemoved = true;
               break; // bullet already erased, move to next one
           }
           else {
               ++enemyIt;
           }
       }

       if (!bulletRemoved)
           ++bulletIt;
   }

   // Check collisions (asteroid vs Earth)  
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

    // Player & asteroids & bullets
    window.draw(player);
    for (auto& a : enemies)
        window.draw(a);
    for (auto& b : bullets)
        window.draw(b);

    // Draw health icons
    for (int i = 0; i < lives; ++i) {
        heartSprite.setPosition(10.f + i * (heartSprite.getGlobalBounds().width + 5.f), 10.f);
        window.draw(heartSprite);
    }

    // Remove Bullets that go out of bounds
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [&](Bullet& b) {
            return b.isOffScreen(windowPtr->getSize());
            }),
        bullets.end()
    );

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
        elapsedTime -= frameTime; 
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
