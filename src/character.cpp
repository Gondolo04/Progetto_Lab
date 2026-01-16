#include "pathfinding/character.h"
#include <SFML/Graphics.hpp>

Character::Character(const Position& startPos, sf::Color color) 
    : position_(startPos), color_(color) {
}

bool Character::moveUp(const Grid& grid) {
    return tryMove(grid, Position(position_.x, position_.y - 1));
}

bool Character::moveDown(const Grid& grid) {
    return tryMove(grid, Position(position_.x, position_.y + 1));
}

bool Character::moveLeft(const Grid& grid) {
    return tryMove(grid, Position(position_.x - 1, position_.y));
}

bool Character::moveRight(const Grid& grid) {
    return tryMove(grid, Position(position_.x + 1, position_.y));
}

bool Character::tryMove(const Grid& grid, const Position& newPos) {
    // Check if the new position is valid and walkable
    if (grid.isInBounds(newPos) && grid.isWalkable(newPos)) {
        position_ = newPos;
        return true;
    }
    return false;
}

void Character::render(sf::RenderWindow& window, float tileSize) const {
    sf::CircleShape characterShape(tileSize / 2.5f);
    characterShape.setFillColor(color_);
    characterShape.setOutlineColor(sf::Color::Black);
    characterShape.setOutlineThickness(2.0f);
    
    // Center the character in the tile
    float offset = (tileSize - characterShape.getRadius() * 2) / 2;
    characterShape.setPosition(sf::Vector2f(
        position_.x * tileSize + offset,
        position_.y * tileSize + offset
    ));
    
    window.draw(characterShape);
}

void Character::handleInput(const Grid& grid) {
    // Handle real-time keyboard input for smooth movement
    static sf::Clock moveClock;
    const float moveDelay = 0.15f; // Delay between moves in seconds
    
    if (moveClock.getElapsedTime().asSeconds() >= moveDelay) {
        bool moved = false;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            moved = moveUp(grid);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || 
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            moved = moveDown(grid);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            moved = moveLeft(grid);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            moved = moveRight(grid);
        }
        
        if (moved) {
            moveClock.restart();
        }
    }
}