#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

// Simple 2D position struct
struct Position {
    int x, y;
    
    Position() : x(0), y(0) {}
    Position(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// Grid cell types
enum class CellType {
    Empty = 0,    // Walkable
    Wall = 1      // Blocked/obstacle
};

class Grid {
public:
    // Constructor - create grid with given dimensions
    Grid(int width, int height);
    
    // Basic grid properties
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    // Cell access
    bool isWalkable(const Position& pos) const; 
    bool isWalkable(int x, int y) const;
    bool isInBounds(const Position& pos) const;
    bool isInBounds(int x, int y) const;
    
    // Modify grid
    void setCell(const Position& pos, CellType type);
    void setCell(int x, int y, CellType type);
    CellType getCell(const Position& pos) const;
    CellType getCell(int x, int y) const;
    
    // For A* pathfinding - get valid neighbors
    std::vector<Position> getNeighbors(const Position& pos) const;
    
    // Rendering
    void render(sf::RenderWindow& window, float tileSize) const;
    
    // Utility - add some test obstacles
    void addTestObstacles();

private:
    int width_, height_;
    std::vector<std::vector<CellType>> cells_;
};