#include "GameOver.hpp"

GameOverScreen::GameOverScreen() {
    window.create(sf::VideoMode(800, 600), "Game Over");
    window.setFramerateLimit(60);

    if (!font.loadFromFile("Base 02.ttf")) {
        // Handle font loading failure
    }

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(90.f, 230.f);
}

void GameOverScreen::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(gameOverText);
        window.display();
    }
}
