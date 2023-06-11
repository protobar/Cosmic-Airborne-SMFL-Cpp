#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <SFML/Graphics.hpp>

class GameOverScreen 
{
private:
    sf::RenderWindow window;
    sf::Text gameOverText;
    sf::Font font;

public:
    GameOverScreen();
    void run();
};

#endif
