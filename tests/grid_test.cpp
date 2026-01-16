#include <gtest/gtest.h>
#include "pathfinding/grid.h"

namespace pathfinding::test {

class GridTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a 5x5 grid for testing
        grid = std::make_unique<Grid>(5, 5);
    }

    std::unique_ptr<Grid> grid;
};

// Test Grid constructor and basic properties
TEST_F(GridTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(grid->getWidth(), 5);
    EXPECT_EQ(grid->getHeight(), 5);
}

// Test initial cell states
TEST_F(GridTest, InitialCellsAreWalkable) {
    // All cells should start as walkable (EMPTY)
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            Position pos{x, y};
            EXPECT_TRUE(grid->isWalkable(pos)) 
                << "Cell (" << x << ", " << y << ") should be walkable";
        }
    }
}

// Test setting and getting cell types
TEST_F(GridTest, SetCellTypes) {
    Position pos{2, 2};
    
    // Set cell to wall
    grid->setCell(pos, CellType::Wall);
    EXPECT_FALSE(grid->isWalkable(pos)) 
        << "Wall cell should not be walkable";
    
    // Set cell back to empty
    grid->setCell(pos, CellType::Empty);
    EXPECT_TRUE(grid->isWalkable(pos)) 
        << "Empty cell should be walkable";
}

// Test bounds checking
TEST_F(GridTest, BoundsCheckingWorks) {
    // Test positions outside the grid
    EXPECT_FALSE(grid->isWalkable({-1, 0})) 
        << "Position outside grid should not be walkable";
    EXPECT_FALSE(grid->isWalkable({0, -1})) 
        << "Position outside grid should not be walkable";
    EXPECT_FALSE(grid->isWalkable({5, 0})) 
        << "Position outside grid should not be walkable";
    EXPECT_FALSE(grid->isWalkable({0, 5})) 
        << "Position outside grid should not be walkable";
    
    // Test valid edge positions
    EXPECT_TRUE(grid->isWalkable({0, 0})) 
        << "Valid edge position should be walkable";
    EXPECT_TRUE(grid->isWalkable({4, 4})) 
        << "Valid edge position should be walkable";
}

// Test neighbor finding for corner position
TEST_F(GridTest, GetNeighborsCornerPosition) {
    Position corner{0, 0};
    auto neighbors = grid->getNeighbors(corner);
    
    // Corner should have exactly 2 neighbors
    EXPECT_EQ(neighbors.size(), 2) 
        << "Corner position should have 2 neighbors";
    
    // Check that neighbors are the expected positions //using set because order of pairs doesn't matter
    std::set<std::pair<int, int>> expectedNeighbors = {{1, 0}, {0, 1}};
    std::set<std::pair<int, int>> actualNeighbors;
    
    for (const auto& neighbor : neighbors) {
        actualNeighbors.insert({neighbor.x, neighbor.y});
    }
    
    EXPECT_EQ(actualNeighbors, expectedNeighbors) 
        << "Corner neighbors should be at positions (1,0) and (0,1)";
}

// Test neighbor finding for center position
TEST_F(GridTest, GetNeighborsCenterPosition) {
    Position center{2, 2};
    auto neighbors = grid->getNeighbors(center);
    
    // Center should have exactly 4 neighbors
    EXPECT_EQ(neighbors.size(), 4) 
        << "Center position should have 4 neighbors";
    
    // Check that neighbors are the expected positions
    std::set<std::pair<int, int>> expectedNeighbors = {
        {1, 2}, {3, 2}, {2, 1}, {2, 3}
    };
    std::set<std::pair<int, int>> actualNeighbors;
    
    for (const auto& neighbor : neighbors) {
        actualNeighbors.insert({neighbor.x, neighbor.y});
    }
    
    EXPECT_EQ(actualNeighbors, expectedNeighbors) 
        << "Center neighbors should be at adjacent positions";
}

// Test neighbor finding with blocked cells
TEST_F(GridTest, GetNeighborsWithBlockedCells) {
    Position center{2, 2};
    
    // Block one neighbor
    grid->setCell({1, 2}, CellType::Wall);
    
    auto neighbors = grid->getNeighbors(center);
    
    // Should now have 3 neighbors instead of 4
    EXPECT_EQ(neighbors.size(), 3) 
        << "Center should have 3 neighbors when one is blocked";
    
    // Verify the blocked position is not in the neighbors list
    for (const auto& neighbor : neighbors) {
        EXPECT_FALSE(neighbor.x == 1 && neighbor.y == 2) 
            << "Blocked neighbor should not be returned";
    }
}

// Test neighbor finding for edge position
TEST_F(GridTest, GetNeighborsEdgePosition) {
    Position edge{0, 2}; // Left edge, middle
    auto neighbors = grid->getNeighbors(edge);
    
    // Edge position should have exactly 3 neighbors
    EXPECT_EQ(neighbors.size(), 3) 
        << "Edge position should have 3 neighbors";
    
    // Check that neighbors are the expected positions
    std::set<std::pair<int, int>> expectedNeighbors = {
        {1, 2}, {0, 1}, {0, 3}
    };
    std::set<std::pair<int, int>> actualNeighbors;
    
    for (const auto& neighbor : neighbors) {
        actualNeighbors.insert({neighbor.x, neighbor.y});
    }
    
    EXPECT_EQ(actualNeighbors, expectedNeighbors) 
        << "Edge neighbors should be at valid adjacent positions";
}

// Test Position struct equality
TEST_F(GridTest, PositionEquality) {
    Position pos1{3, 4};
    Position pos2{3, 4};
    Position pos3{4, 3};
    
    EXPECT_TRUE(pos1.x == pos2.x && pos1.y == pos2.y) 
        << "Positions with same coordinates should be equal";
    EXPECT_FALSE(pos1.x == pos3.x && pos1.y == pos3.y) 
        << "Positions with different coordinates should not be equal";
}

// Test grid with different dimensions
TEST_F(GridTest, DifferentDimensionsWork) {
    Grid smallGrid(2, 3); // 2 wide, 3 tall
    
    EXPECT_EQ(smallGrid.getWidth(), 2);
    EXPECT_EQ(smallGrid.getHeight(), 3);
    
    // Test all valid positions are walkable
    EXPECT_TRUE(smallGrid.isWalkable({0, 0}));
    EXPECT_TRUE(smallGrid.isWalkable({1, 2}));
    
    // Test invalid positions
    EXPECT_FALSE(smallGrid.isWalkable({2, 0})); // Outside width
    EXPECT_FALSE(smallGrid.isWalkable({0, 3})); // Outside height
}

// Test getCell method
TEST_F(GridTest, GetCellReturnsCorrectType) {
    // Test default state
    EXPECT_EQ(grid->getCell({2, 2}), CellType::Empty) 
        << "Default cell should be Empty";
    
    // Set cell to wall and test retrieval
    grid->setCell({2, 2}, CellType::Wall);
    EXPECT_EQ(grid->getCell({2, 2}), CellType::Wall) 
        << "Cell should be Wall after setting";
    
    // Set back to empty and test
    grid->setCell({2, 2}, CellType::Empty);
    EXPECT_EQ(grid->getCell({2, 2}), CellType::Empty) 
        << "Cell should be Empty after resetting";
    
    // Test out of bounds returns Wall
    EXPECT_EQ(grid->getCell({-1, 0}), CellType::Wall) 
        << "Out of bounds should return Wall";
    EXPECT_EQ(grid->getCell({5, 0}), CellType::Wall) 
        << "Out of bounds should return Wall";
}

// Test isInBounds method explicitly
TEST_F(GridTest, IsInBoundsWorksCorrectly) {
    // Test valid positions
    EXPECT_TRUE(grid->isInBounds({0, 0})) 
        << "Top-left corner should be in bounds";
    EXPECT_TRUE(grid->isInBounds({4, 4})) 
        << "Bottom-right corner should be in bounds";
    EXPECT_TRUE(grid->isInBounds({2, 3})) 
        << "Center position should be in bounds";
    
    // Test invalid positions
    EXPECT_FALSE(grid->isInBounds({-1, 0})) 
        << "Negative x should be out of bounds";
    EXPECT_FALSE(grid->isInBounds({0, -1})) 
        << "Negative y should be out of bounds";
    EXPECT_FALSE(grid->isInBounds({5, 0})) 
        << "x >= width should be out of bounds";
    EXPECT_FALSE(grid->isInBounds({0, 5})) 
        << "y >= height should be out of bounds";
    EXPECT_FALSE(grid->isInBounds({10, 10})) 
        << "Far out of bounds should be out of bounds";
}

} 