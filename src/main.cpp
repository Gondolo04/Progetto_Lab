#include <SFML/Graphics.hpp>
#include <iostream>
#include "pathfinding/grid.h"

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Grid Test - A* Game");
    window.setFramerateLimit(60);
    
    // Create a test grid (40x30 fits nicely in 800x600 with 20px tiles)
    Grid grid(40, 30);
    float tileSize = 20.0f;
    
    // Add some test obstacles to see the grid
    grid.addTestObstacles();
    
    std::cout << "Grid created successfully!" << std::endl;
    std::cout << "Grid size: " << grid.getWidth() << "x" << grid.getHeight() << std::endl;
    std::cout << "Controls: ESC to close, Left-click to add walls, Right-click to remove walls" << std::endl;
    
    // Test some basic grid functionality
    std::cout << "Testing grid functions:" << std::endl;
    std::cout << "Position (0,0) walkable: " << (grid.isWalkable(0, 0) ? "Yes" : "No") << std::endl;
    std::cout << "Position (10,8) walkable: " << (grid.isWalkable(10, 8) ? "Yes" : "No") << " (should be No - wall)" << std::endl;
    
    // Test neighbors
    Position testPos(5, 5);
    auto neighbors = grid.getNeighbors(testPos);
    std::cout << "Position (5,5) has " << neighbors.size() << " walkable neighbors" << std::endl;
    
    // Main loop
    while (window.isOpen()) {
        // Handle events
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
            
            // Mouse interaction - click to add/remove walls
            if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                int gridX = static_cast<int>(mousePressed->position.x / tileSize);
                int gridY = static_cast<int>(mousePressed->position.y / tileSize);
                
                if (grid.isInBounds(gridX, gridY)) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        // Left click - add wall
                        grid.setCell(gridX, gridY, CellType::Wall);
                        std::cout << "Added wall at (" << gridX << ", " << gridY << ")" << std::endl;
                    } else if (mousePressed->button == sf::Mouse::Button::Right) {
                        // Right click - remove wall
                        grid.setCell(gridX, gridY, CellType::Empty);
                        std::cout << "Removed wall at (" << gridX << ", " << gridY << ")" << std::endl;
                    }
                }
            }
        }
        
        // Clear screen
        window.clear(sf::Color::Black);
        
        // Draw the grid
        grid.render(window, tileSize);
        
        // Display
        window.display();
    }
    
    std::cout << "Grid test complete!" << std::endl;
    return 0;
}