#pragma once
#include "grid.h"
#include "pathfinder.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
    
    // A* Pathfinding
    bool findPathTo(const Grid& grid, const Position& target);
    void followPath();  // Move one step along the current path
    void clearPath();   // Clear the current path
    bool hasPath() const { return !currentPath_.empty(); }
    const std::vector<Position>& getCurrentPath() const { return currentPath_; }

private:
    Position position_;
    sf::Color color_;
    
    // Pathfinding members
    Pathfinder pathfinder_;
    std::vector<Position> currentPath_;
    size_t pathIndex_;  // Current index in the path
    
    bool tryMove(const Grid& grid, const Position& newPos);
};