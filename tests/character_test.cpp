#include <gtest/gtest.h>
#include "pathfinding/character.h"
#include "pathfinding/grid.h"
#include <SFML/Graphics.hpp>

namespace pathfinding::test {

class CharacterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a 5x5 grid for testing
        grid = std::make_unique<Grid>(5, 5);
        
        // Create a character at position (1, 1)
        startPos = Position{1, 1};
        character = std::make_unique<Character>(startPos, sf::Color::Green);
    }

    std::unique_ptr<Grid> grid;
    std::unique_ptr<Character> character;
    Position startPos;
};

// Test Character constructor and initial state
TEST_F(CharacterTest, ConstructorInitializesCorrectly) {
    // Test position initialization
    EXPECT_EQ(character->getPosition().x, 1);
    EXPECT_EQ(character->getPosition().y, 1);
    
    // Test that character has no initial path
    EXPECT_FALSE(character->hasPath());
    EXPECT_TRUE(character->getCurrentPath().empty());
}

// Test constructor with different colors
TEST_F(CharacterTest, ConstructorWithDifferentColors) {
    Character redCharacter(Position{2, 2}, sf::Color::Red);
    EXPECT_EQ(redCharacter.getPosition().x, 2);
    EXPECT_EQ(redCharacter.getPosition().y, 2);
}

// Test position getter and setter
TEST_F(CharacterTest, PositionGetterAndSetter) {
    Position newPos{3, 4};
    character->setPosition(newPos);
    
    EXPECT_EQ(character->getPosition().x, 3);
    EXPECT_EQ(character->getPosition().y, 4);
}

// Test color setter
TEST_F(CharacterTest, ColorSetter) {
    character->setColor(sf::Color::Blue);

    EXPECT_EQ(character->getColor(), sf::Color::Blue);

    character->setColor(sf::Color::Red);

    EXPECT_EQ(character->getColor(), sf::Color::Red);

    character->setColor(sf::Color::Yellow);
    
    EXPECT_EQ(character->getColor(), sf::Color::Yellow);
}

// Test successful movement in all directions
TEST_F(CharacterTest, SuccessfulMovement) {
    Position originalPos = character->getPosition();
    
    // Test move up
    EXPECT_TRUE(character->moveUp(*grid));
    EXPECT_EQ(character->getPosition().x, originalPos.x);
    EXPECT_EQ(character->getPosition().y, originalPos.y - 1);
    
    // Test move down (back to original)
    EXPECT_TRUE(character->moveDown(*grid));
    EXPECT_EQ(character->getPosition(), originalPos);
    
    // Test move left
    EXPECT_TRUE(character->moveLeft(*grid));
    EXPECT_EQ(character->getPosition().x, originalPos.x - 1);
    EXPECT_EQ(character->getPosition().y, originalPos.y);
    
    // Test move right (back to original)
    EXPECT_TRUE(character->moveRight(*grid));
    EXPECT_EQ(character->getPosition(), originalPos);
}

// Test movement blocked by grid boundaries
TEST_F(CharacterTest, MovementBlockedByBoundaries) {
    character->setPosition(Position{0, 0});
    
    // Try to move further up and left - should be blocked
    EXPECT_FALSE(character->moveUp(*grid));
    EXPECT_FALSE(character->moveLeft(*grid));
    EXPECT_EQ(character->getPosition().x, 0);
    EXPECT_EQ(character->getPosition().y, 0);
    
    // Move character to bottom-right corner (4, 4)
    character->setPosition(Position{4, 4});
    
    // Try to move further down and right - should be blocked
    EXPECT_FALSE(character->moveDown(*grid));
    EXPECT_FALSE(character->moveRight(*grid));
    EXPECT_EQ(character->getPosition().x, 4);
    EXPECT_EQ(character->getPosition().y, 4);
}

// Test movement blocked by walls
TEST_F(CharacterTest, MovementBlockedByWalls) {
    Position wallPos{1, 0};
    grid->setCell(wallPos, CellType::Wall);
    
    // Try to move up into the wall - should be blocked
    EXPECT_FALSE(character->moveUp(*grid));
    EXPECT_EQ(character->getPosition(), startPos); // Should remain at original position
    
    Position rightWallPos{2, 1};
    grid->setCell(rightWallPos, CellType::Wall);
    
    // Try to move right into the wall - should be blocked
    EXPECT_FALSE(character->moveRight(*grid));
    EXPECT_EQ(character->getPosition(), startPos); // Should remain at original position
}

// Test pathfinding to a valid target
TEST_F(CharacterTest, FindPathToValidTarget) {
    Position target{3, 3};
    
    EXPECT_TRUE(character->findPathTo(*grid, target));
    EXPECT_TRUE(character->hasPath());
    EXPECT_FALSE(character->getCurrentPath().empty());
}


// Test pathfinding to blocked target
TEST_F(CharacterTest, FindPathToBlockedTarget) {
    // Create a completely walled-off target
    Position target{3, 3};
    grid->setCell(Position{3, 3}, CellType::Wall);
    
    EXPECT_FALSE(character->findPathTo(*grid, target));
    EXPECT_FALSE(character->hasPath());
    EXPECT_TRUE(character->getCurrentPath().empty());
}

// Test pathfinding to unreachable target
TEST_F(CharacterTest, FindPathToUnreachableTarget) {
    // Create a wall barrier that blocks access to the target
    Position target{3, 3};
    
    // Create a wall around the target
    grid->setCell(Position{2, 2}, CellType::Wall);
    grid->setCell(Position{2, 3}, CellType::Wall);
    grid->setCell(Position{2, 4}, CellType::Wall);
    grid->setCell(Position{3, 2}, CellType::Wall);
    grid->setCell(Position{3, 4}, CellType::Wall);
    grid->setCell(Position{4, 2}, CellType::Wall);
    grid->setCell(Position{4, 3}, CellType::Wall);
    grid->setCell(Position{4, 4}, CellType::Wall);
    
    EXPECT_FALSE(character->findPathTo(*grid, target));
    EXPECT_FALSE(character->hasPath());
}


// Test clearing path
TEST_F(CharacterTest, ClearPath) {
    Position target{3, 3};
    
    character->findPathTo(*grid, target);
    
    // Clear the path
    character->clearPath();
    EXPECT_FALSE(character->hasPath());
    EXPECT_TRUE(character->getCurrentPath().empty());
}

// Test path completion
TEST_F(CharacterTest, PathCompletion) {
    Position target{2, 1}; // Adjacent position
    
    EXPECT_TRUE(character->findPathTo(*grid, target));
    EXPECT_TRUE(character->hasPath());
    
    // Follow the entire path
    while (character->hasPath()) {
        character->followPath();
    }
    
    // Should have reached target and cleared path
    EXPECT_EQ(character->getPosition(), target);
    EXPECT_FALSE(character->hasPath());
}
} 