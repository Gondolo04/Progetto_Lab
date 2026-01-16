#pragma once
#include "grid.h"
#include <SFML/Graphics.hpp>

class Character {
public:
    // Constructor
    Character(const Position& startPos, sf::Color color = sf::Color::Green);
    
    // Movement
    bool moveUp(const Grid& grid);
    bool moveDown(const Grid& grid);
    bool moveLeft(const Grid& grid);
    bool moveRight(const Grid& grid);
    
    // Position access
    const Position& getPosition() const {
         return position_;
    } 

    void setPosition(const Position& pos) {
         position_ = pos;
    }
    
    // Change color
    void setColor(const sf::Color& color) {
         color_ = color;
    }
    
    // Rendering
    void render(sf::RenderWindow& window, float tileSize) const;
    
    // Input handling
    void handleInput(const Grid& grid);

private:
    Position position_;
    sf::Color color_;
    
    bool tryMove(const Grid& grid, const Position& newPos);
};