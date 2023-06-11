#pragma once

#include <SFML/Graphics.hpp>

class Enemy 
{
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;


public:
    Enemy(sf::Texture& texture, const sf::Vector2f& startPosition, const sf::Vector2f& startVelocity);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& newPosition); // Added method

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};
