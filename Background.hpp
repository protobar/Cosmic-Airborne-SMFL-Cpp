#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Background(const std::string& imagePath);
    void draw(sf::RenderWindow& window);
};

#endif  // BACKGROUND_HPP
