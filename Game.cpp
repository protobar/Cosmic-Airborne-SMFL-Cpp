#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Background.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "GameOver.hpp"
#include <iostream>


// Aggregated class
class AggregatedEnemyDied {
public:
    void printMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
};

// Base class
class BaseEnemyClass {
protected:
    AggregatedEnemyDied aggregator; // Aggregated class instance

public:
    void sayHello() {
        aggregator.printMessage("Eneny, ");
    }
};

// Derived class
class DerivedEnemyClass : public BaseEnemyClass {
public:
    void sayWorld() {
        aggregator.printMessage("Died!");
    }
};

class Game
{
private:
    sf::RenderWindow window;
    Background background;
    Player player1;
    Player player2;
    sf::RectangleShape bulletShape; // Bullet shape
    GameOverScreen gameOverScreen;

    sf::Vector2f bulletVelocity; // Velocity of the bullet
    sf::Vector2f bulletStartPosition; // Starting position of the bullet
    sf::Color bulletColor; // Color of the bullet

    enum class GameState
    {
        Running,
        GameOver
    };
    GameState gameState;
    //Font
    sf::Font font;
    sf::Music shootSound, shootSound2;
    //GameOver
    bool gameOver;

    //Lives and scores
    int player1Lives;
    int player1Score;
    int player2Lives;
    int player2Score;

    bool player1BulletFired; // Flag indicating if player 1 bullet is fired
    bool player2BulletFired; // Flag indicating if player 2 bullet is fired

    std::vector<Enemy> enemies; // Vector to store enemies

public:
    Game(const std::string& backgroundImage, const std::string& player1Image, const std::string& player2Image, const std::string& enemyImage)
        : window(sf::VideoMode(800, 600), "Space Shooter"),
        background(backgroundImage),
        player1(player1Image, sf::Vector2f(100.f, 300.f), 0.5f, 1.f),
        player2(player2Image, sf::Vector2f(700.f, 300.f), 0.5f, 1.f),
        bulletShape(sf::Vector2f(10.f, 10.f)), // Set bullet size
        bulletVelocity(0.f, -1.f), // Set bullet velocity
        bulletStartPosition(-100.f, -100.f), // Initialize bullet start position off-screen
        bulletColor(sf::Color::White), // Set default bullet color
        player1BulletFired(false), // Initialize player 1 bullet flag
        player2BulletFired(false), // Initialize player 2 bullet flag
        player1Lives(3),
        player1Score(0),
        player2Lives(3),
        player2Score(0)
    {
        if (!font.loadFromFile("ARCADECLASSIC.ttf")) {
            std::cout << "Failed to load font!" << std::endl;
            // Handle font loading failure
        }
        if (!shootSound.openFromFile("music/pew.ogg"))
        {
            return ;
        }
        if (!shootSound2.openFromFile("music/pew_2.ogg"))
        {
            return ;
        }
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Player 1 movement
        sf::Vector2f p1Direction;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            p1Direction.y -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            p1Direction.y += 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            p1Direction.x -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            p1Direction.x += 0.5f;
        player1.move(p1Direction);

        // Player 1 shooting
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player1BulletFired)
        {
            shootBullet(player1.getPosition(), player1.getSprite().getGlobalBounds().width, sf::Color::Magenta);
            player1BulletFired = true;
            shootSound.setVolume(100);
            shootSound.play();
        }

        // Player 2 movement
        sf::Vector2f p2Direction;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            p2Direction.y -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            p2Direction.y += 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            p2Direction.x -= 0.5f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            p2Direction.x += 0.5f;
        player2.move(p2Direction);

        // Player 2 shooting
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && !player2BulletFired) {
            shootBullet(player2.getPosition(), player2.getSprite().getGlobalBounds().width, sf::Color::Cyan);
            player2BulletFired = true;
            shootSound2.setVolume(100);
            shootSound2.play();
        }
    }

    void update() {
        if (gameState == GameState::Running) {
            // Update bullet position
            updateBullet();

            // Update enemy positions
            updateEnemies();

            // Check for collision between bullets and enemies
            checkBulletEnemyCollision();

            // Check for collision between players and enemies
            checkPlayerEnemyCollision();

            // Clean up enemies that are off-screen
            cleanUpEnemies();

            // Spawn new enemies if needed
            spawnEnemies();
        }
        if (gameOver) {
            // Transition to the game over state
            gameState = GameState::GameOver;
            GameOverScreen gameOverScreen;
            gameOverScreen.run();
            window.close();
        }
    }


    void updateBullet() {
        if (player1BulletFired) {
            bulletStartPosition += bulletVelocity;

            // Check if bullet is off-screen
            if (bulletStartPosition.y < 0) {
                player1BulletFired = false;
                bulletStartPosition = sf::Vector2f(-1000.f, -1000.f); // Move the bullet off-screen
            }
        }

        if (player2BulletFired) {
            bulletStartPosition += bulletVelocity;

            // Check if bullet is off-screen
            if (bulletStartPosition.y < 0) {
                player2BulletFired = false;
                bulletStartPosition = sf::Vector2f(-1000.f, -1000.f); // Move the bullet off-screen
            }
        }
    }

    void updateEnemies() {
        float deltaTime = 0.1f; // Adjust this value based on your game's frame rate

        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
    }

    void checkBulletEnemyCollision() {
        for (auto& enemy : enemies) {
            if (enemy.getBounds().intersects(bulletShape.getGlobalBounds())) {
                // Enemy hit by a bullet
                // Handle enemy destruction or damage here
                enemy.setPosition(sf::Vector2f(-100.f, -100.f)); // Move the enemy off-screen
                bulletStartPosition = sf::Vector2f(-100.f, -100.f); // Move the bullet off-screen
                player1BulletFired = false;
                player2BulletFired = false;


                // Increase the score for the respective player
                if (bulletColor == sf::Color::Magenta) {
                    player1Score += 10;
                }
                else if (bulletColor == sf::Color::Cyan) {
                    player2Score += 10;
                }
                break; // Exit the loop after the first collision
            }
        }
    }

    void checkPlayerEnemyCollision() {
        sf::FloatRect player1Bounds = player1.getSprite().getGlobalBounds();
        sf::FloatRect player2Bounds = player2.getSprite().getGlobalBounds();

        for (auto& enemy : enemies) {
            sf::FloatRect enemyBounds = enemy.getBounds();

            if (player1Bounds.intersects(enemyBounds)) {
                // Player 1 collided with an enemy
                player1Lives--;
                enemy.setPosition(sf::Vector2f(-100.f, -100.f)); // Move the enemy off-screen
            }

            if (player2Bounds.intersects(enemyBounds)) {
                // Player 2 collided with an enemy
                player2Lives--;
                enemy.setPosition(sf::Vector2f(-100.f, -100.f)); // Move the enemy off-screen
            }
        }

        // Check if any player has run out of lives
        if (player1Lives <= 0 && player2Lives <= 0) {
            // Both players have zero lives - GAME OVER

            gameOver = true;
        }
        else
        {
            if (player1Lives <= 0)
            {
                // Player 1 has zero lives - remove from screen
                player1.move(sf::Vector2f(-100.f, -100.f)); // Move player 1 off-screen
            }

            if (player2Lives <= 0)
            {
                // Player 2 has zero lives - remove from screen
                player2.move(sf::Vector2f(-100.f, -100.f)); // Move player 2 off-screen
            }
        }
    }


    void cleanUpEnemies() {
        for (auto& enemy : enemies) {
            if (enemy.getPosition().y > window.getSize().y) {
                enemy.setPosition(sf::Vector2f(-100.f, -100.f)); // Move the enemy off-screen
            }
        }
    }

    void spawnEnemies() {
        static const float SPAWN_INTERVAL = 150.0f; // Interval between enemy spawns
        static const int MAX_ENEMIES = 100; // Maximum number of enemies on the screen
        static float spawnTimer = 0.0f;

        spawnTimer += 0.1f; // Adjust this value based on your game's frame rate

        if (spawnTimer >= SPAWN_INTERVAL && enemies.size() < MAX_ENEMIES) {
            sf::Texture enemyTexture;
            if (!enemyTexture.loadFromFile("Resources/enemy.png")) {
                std::cout << "Failed to load enemy texture!" << std::endl;
                return;
            }

            float spawnPositionX = static_cast<float>(std::rand() % (window.getSize().x - 100) + 50);
            float enemySpeed = 0.5f; // Adjust the speed of the spawned enemy as needed

            enemies.push_back(Enemy(enemyTexture, sf::Vector2f(spawnPositionX, -100.f), sf::Vector2f(0.f, enemySpeed)));
            spawnTimer = 0.0f; // Reset the spawn timer
        }

        // Remove off-screen enemies
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [&](const Enemy& enemy) {
            return enemy.getPosition().y > window.getSize().y;
            }), enemies.end());
    }


    void render() 
    {
        window.clear();
        background.draw(window);
        player1.draw(window);
        player2.draw(window);

        // Draw bullet
        if (player1BulletFired || player2BulletFired) 
{
            bulletShape.setPosition(bulletStartPosition);
            bulletShape.setFillColor(bulletColor);
            window.draw(bulletShape);
        }

        // Draw enemies
        for (auto& enemy : enemies) 
        {
            enemy.draw(window);
        }

        // Draw player 1's score and lives
        sf::Text player1ScoreText;
        player1ScoreText.setFont(font); // Set the font (you need to load a font file)
        player1ScoreText.setCharacterSize(20); // Set the font size
        player1ScoreText.setFillColor(sf::Color::Magenta);
        player1ScoreText.setPosition(10.f, 10.f); // Upper left corner of the screen
        player1ScoreText.setString("Score: " + std::to_string(player1Score) + "\nLives: " + std::to_string(player1Lives));
        window.draw(player1ScoreText);

        // Draw player 2's score and lives
        sf::Text player2ScoreText;
        player2ScoreText.setFont(font); // Set the font (you need to load a font file)
        player2ScoreText.setCharacterSize(20); // Set the font size
        player2ScoreText.setFillColor(sf::Color::Cyan);
        player2ScoreText.setPosition(window.getSize().x - 100.f, 10.f); // Upper right corner of the screen
        player2ScoreText.setString("Score: " + std::to_string(player2Score) + "\nLives: " + std::to_string(player2Lives));
        window.draw(player2ScoreText);

        window.display();
    }

    void shootBullet(const sf::Vector2f& playerPosition, float playerWidth, const sf::Color& bulletColor) {
        bulletStartPosition.x = playerPosition.x + playerWidth / 2.f - bulletShape.getSize().x / 2.f;
        bulletStartPosition.y = playerPosition.y - bulletShape.getSize().y;

        this->bulletColor = bulletColor;
    }

};



