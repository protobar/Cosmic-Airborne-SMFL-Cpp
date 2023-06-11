#include "Background.hpp"

Background::Background(const std::string& imagePath) {
    texture.loadFromFile(imagePath);
    sprite.setTexture(texture);
}

void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
