#include "Enemy.hpp"

Enemy::Enemy(sf::Texture& texture, const sf::Vector2f& startPosition, const sf::Vector2f& startVelocity)
    : position(startPosition), velocity(startVelocity)
{
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(position);
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
}

void Enemy::update(float deltaTime) {
    position += velocity * deltaTime;
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
