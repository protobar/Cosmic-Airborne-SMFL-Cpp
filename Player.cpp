#include "Player.hpp"

Player::Player(const std::string& imagePath, const sf::Vector2f& startPosition, float playerSpeed, float reducerSize)
    : speed(playerSpeed), sizeReducer(reducerSize)
{
    texture.loadFromFile(imagePath);
    sprite.setTexture(texture);
    sprite.setPosition(startPosition);
    position = startPosition;
    bullet.setSize(sf::Vector2f(10.f, 30.f));
    bullet.setFillColor(sf::Color::White);
}

void Player::move(const sf::Vector2f& direction) {
    position += direction * speed;
    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::shootBullet(sf::RenderWindow& window) {
    sf::RectangleShape newBullet(bullet);
    newBullet.setPosition(position);
    window.draw(newBullet);
}

void Player::setSize(float newSize) {
    sprite.setScale(newSize, newSize);
}

void Player::reduceSize() {
    sprite.setScale(sprite.getScale().x * sizeReducer, sprite.getScale().y * sizeReducer);
}

void Player::increaseSize() {
    sprite.setScale(sprite.getScale().x / sizeReducer, sprite.getScale().y / sizeReducer);
}

sf::Vector2f Player::getPosition() const {
    return position;
}

const sf::Sprite& Player::getSprite() const {
    return sprite;
}
