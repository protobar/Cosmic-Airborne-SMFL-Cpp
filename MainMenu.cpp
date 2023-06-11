#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include "Game.cpp"

enum class MenuState { MainMenu, HowToPlay };

sf::Color getRandomNeonColor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    int r = dis(gen);
    int g = dis(gen);
    int b = dis(gen);

    return sf::Color(r, g, b);
}

void animateText(sf::Text& text, float time)
{
    float scale = 1.0f + std::sin(time * 2) * 0.1f; // Scale animation
    float rotation = std::sin(time * 3) * 8; // Rotation animation
    float offsetX = std::sin(time * 1.5f) * 10; // Offset animation

    text.setScale(scale, scale);
    text.setRotation(rotation);
    text.setPosition(150 + offsetX, text.getPosition().y);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cosmic Airborne");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("Base 02.ttf")) 
    {
        return -1;
    }

    MenuState menuState = MenuState::MainMenu;

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(60);
    titleText.setString("Cosmic Airborne");
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2);
    titleText.setPosition(100, 100);

    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(40);
    startText.setString("Start");
    startText.setOutlineColor(sf::Color::Black);
    startText.setOutlineThickness(2);
    startText.setPosition(350, 250);

    sf::Text howToPlayText;
    howToPlayText.setFont(font);
    howToPlayText.setCharacterSize(40);
    howToPlayText.setString("How to Play");
    howToPlayText.setOutlineColor(sf::Color::Black);
    howToPlayText.setOutlineThickness(2);
    howToPlayText.setPosition(270, 340);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(40);
    exitText.setString("Exit");
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(2);
    exitText.setPosition(370, 430);

    sf::Text instructionsTitle;
    instructionsTitle.setFont(font);
    instructionsTitle.setCharacterSize(50);
    instructionsTitle.setString("How to Play");
    instructionsTitle.setOutlineColor(sf::Color::Black);
    instructionsTitle.setOutlineThickness(2);
    instructionsTitle.setPosition(280, 180);

    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(30);
    instructionsText.setString("Player 1:\nUse arrow keys to move\nPress Spacebar to shoot\n\nPlayer 2:\nUse WASD to move\nPress Q to shoot");
    instructionsText.setOutlineColor(sf::Color::Black);
    instructionsText.setOutlineThickness(2);
    instructionsText.setPosition(160, 260);

    sf::Music backgroundMusic, bgM;
    if (!backgroundMusic.openFromFile("music/music.ogg")) {
        return -1;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(50);
    backgroundMusic.play();
    
    
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (menuState == MenuState::MainMenu)
                {
                    if (startText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator

                        Game game("Resources/background.png", "Resources/player1.png", "Resources/player2.png", "Resources/enemy.png");
                        game.run();
                        DerivedEnemyClass obj;
                        obj.sayHello();
                        obj.sayWorld();
                       

                        return 0;
                    }
                    else if (howToPlayText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        menuState = MenuState::HowToPlay;
                    }
                    else if (exitText.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        window.close();
                    }
                }
                else if (menuState == MenuState::HowToPlay)
                {
                    menuState = MenuState::MainMenu;
                }
            }
        }

        window.clear(sf::Color::Black);

        float time = clock.getElapsedTime().asSeconds();

        if (menuState == MenuState::MainMenu)
        {
            animateText(titleText, time); // Animate title text
            titleText.setFillColor(getRandomNeonColor()); // Random neon color
            startText.setFillColor(getRandomNeonColor()); // Random neon color
            howToPlayText.setFillColor(getRandomNeonColor()); // Random neon color
            exitText.setFillColor(getRandomNeonColor()); // Random neon color

            window.draw(titleText);
            window.draw(startText);
            window.draw(howToPlayText);
            window.draw(exitText);
        }
        else if (menuState == MenuState::HowToPlay)
        {
            instructionsTitle.setScale(1.0f, 1.0f); // Reset scale for instructions title
            animateText(instructionsTitle, time); // Animate instructions title
            instructionsTitle.setFillColor(getRandomNeonColor()); // Random neon color
            instructionsText.setFillColor(getRandomNeonColor()); // Random neon color

            window.draw(instructionsTitle);
            window.draw(instructionsText);
        }

        window.display();
    }

    return 0;
}