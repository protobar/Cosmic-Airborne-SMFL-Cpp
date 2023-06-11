#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    float speed;
    float sizeReducer;
    sf::RectangleShape bullet;

public:
    Player(const std::string& imagePath, const sf::Vector2f& startPosition, float playerSpeed, float reducerSize);

    void move(const sf::Vector2f& direction);
    void draw(sf::RenderWindow& window);
    void shootBullet(sf::RenderWindow& window);
    void setSize(float newSize);
    void reduceSize();
    void increaseSize();
    sf::Vector2f getPosition() const;
    const sf::Sprite& getSprite() const;
};

#endif  // PLAYER_HPP
