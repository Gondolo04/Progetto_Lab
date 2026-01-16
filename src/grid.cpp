#include "pathfinding/grid.h"

Grid::Grid(int width, int height) : width_(width), height_(height) {
    // Initialize grid with all empty cells
    cells_.resize(height_, std::vector<CellType>(width_, CellType::Empty));
}

bool Grid::isWalkable(const Position& pos) const {
    return isWalkable(pos.x, pos.y);
}

bool Grid::isWalkable(int x, int y) const {
    if (!isInBounds(x, y)) {
        return false; // Out of bounds = not walkable
    }
    return cells_[y][x] == CellType::Empty;
}

bool Grid::isInBounds(const Position& pos) const {
    return isInBounds(pos.x, pos.y);
}

bool Grid::isInBounds(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void Grid::setCell(const Position& pos, CellType type) {
    setCell(pos.x, pos.y, type);
}

void Grid::setCell(int x, int y, CellType type) {
    if (isInBounds(x, y)) {
        cells_[y][x] = type;
    }
}

CellType Grid::getCell(const Position& pos) const {
    return getCell(pos.x, pos.y);
}

CellType Grid::getCell(int x, int y) const {
    if (!isInBounds(x, y)) {
        return CellType::Wall; // Treat out of bounds as walls
    }
    return cells_[y][x];
}

std::vector<Position> Grid::getNeighbors(const Position& pos) const {
    std::vector<Position> neighbors;
    
    // Check 4 cardinal directions (for A* pathfinding)
    std::vector<Position> directions = {
        {0, -1},  // North
        {1, 0},   // East
        {0, 1},   // South
        {-1, 0}   // West
    };
    
    for (const auto& dir : directions) {
        Position neighbor(pos.x + dir.x, pos.y + dir.y);
        if (isWalkable(neighbor)) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

void Grid::render(sf::RenderWindow& window, float tileSize) const {
    sf::RectangleShape tile(sf::Vector2f(tileSize - 1, tileSize - 1));
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            tile.setPosition({x * tileSize, y * tileSize});
            
            // Color based on cell type
            switch (cells_[y][x]) {
                case CellType::Empty:
                    tile.setFillColor(sf::Color::White);
                    break;
                case CellType::Wall:
                    tile.setFillColor(sf::Color::Red);
                    break;
            }
            
            window.draw(tile);
        }
    }
}

void Grid::addTestObstacles() {
    // Add some walls for testing pathfinding
    
    // Horizontal wall
    for (int x = 5; x < 15; ++x) {
        setCell(x, 8, CellType::Wall);
    }
    
    // Vertical wall
    for (int y = 3; y < 12; ++y) {
        setCell(12, y, CellType::Wall);
    }
    
    // L-shaped obstacle
    for (int x = 18; x < 25; ++x) {
        setCell(x, 5, CellType::Wall);
    }
    for (int y = 5; y < 10; ++y) {
        setCell(18, y, CellType::Wall);
    }
}