#include <SFML/Graphics.hpp>
#include <iostream>
#include "pathfinding/grid.h"
#include "pathfinding/character.h"

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Grid Test - A* Game");
    window.setFramerateLimit(60);
    
    // Create a test grid (40x30 fits nicely in 800x600 with 20px tiles)
    Grid grid(40, 30);
    float tileSize = 20.0f;
    
    // Add some test obstacles to see the grid
    grid.addTestObstacles();
    
    // Create a character starting at position (1, 1)
    Character player(Position(1, 1), sf::Color::Green);
    
    std::cout << "Grid created successfully!" << std::endl;
    std::cout << "Grid size: " << grid.getWidth() << "x" << grid.getHeight() << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD or Arrow Keys to move character" << std::endl;
    std::cout << "  1-5 keys to change character color" << std::endl;
    std::cout << "  ESC to close" << std::endl;
    std::cout << "  Left-click to add walls, Right-click to remove walls" << std::endl;

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
                
                // Color changing with number keys
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Num1:
                        player.setColor(sf::Color::Green);
                        std::cout << "Character color changed to Green" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num2:
                        player.setColor(sf::Color::Blue);
                        std::cout << "Character color changed to Blue" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num3:
                        player.setColor(sf::Color::Red);
                        std::cout << "Character color changed to Red" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num4:
                        player.setColor(sf::Color::Yellow);
                        std::cout << "Character color changed to Yellow" << std::endl;
                        break;
                    case sf::Keyboard::Key::Num5:
                        player.setColor(sf::Color::Magenta);
                        std::cout << "Character color changed to Magenta" << std::endl;
                        break;
                    default:
                        break;
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
        
        // Handle character movement
        player.handleInput(grid);
        
        // Clear screen
        window.clear(sf::Color::Black);
        
        // Draw the grid
        grid.render(window, tileSize);
        
        // Draw the character
        player.render(window, tileSize);
        
        // Display
        window.display();
    }
    
    std::cout << "Grid test complete!" << std::endl;
    return 0;
}