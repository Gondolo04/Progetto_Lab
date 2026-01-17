#include <gtest/gtest.h>
#include "pathfinding/pathfinder.h"
#include "pathfinding/grid.h"

namespace pathfinding::test {

class PathfinderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a 5x5 grid for testing
        grid = std::make_unique<Grid>(5, 5);
        pathfinder = std::make_unique<Pathfinder>();
    }

    std::unique_ptr<Grid> grid;
    std::unique_ptr<Pathfinder> pathfinder;
};

// Test basic pathfinding with no obstacles
TEST_F(PathfinderTest, BasicPathfindingNoObstacles) {
    Position start{0, 0};
    Position goal{2, 2};
    std::vector<Position> path;
    
    EXPECT_TRUE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), start);
    EXPECT_EQ(path.back(), goal);
    EXPECT_GT(pathfinder->getLastPathCost(), 0.0f);
    EXPECT_GT(pathfinder->getLastSearchSteps(), 0);
}

// Test pathfinding with simple obstacle
TEST_F(PathfinderTest, PathfindingWithObstacle) {
    Position start{0, 1};
    Position goal{2, 1};
    std::vector<Position> path;
    
    // Place a wall in the direct path
    grid->setCell(Position{1, 1}, CellType::Wall);
    
    EXPECT_TRUE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front(), start);
    EXPECT_EQ(path.back(), goal);
    
    // Path should be longer than direct route due to obstacle
    EXPECT_GT(path.size(), 3); // More than direct path of 3 steps
    
    // Verify path doesn't go through the wall
    for (const auto& pos : path) {
        EXPECT_NE(pos, Position(1, 1));
    }
}

// Test pathfinding to same position
TEST_F(PathfinderTest, SamePositionPath) {
    Position start{2, 2};
    Position goal{2, 2}; // Same position
    std::vector<Position> path;
    
    EXPECT_TRUE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_EQ(path.size(), 1);
    EXPECT_EQ(path[0], start);
    EXPECT_FLOAT_EQ(pathfinder->getLastPathCost(), 0.0f);
}

// Test pathfinding to unreachable position
TEST_F(PathfinderTest, UnreachableDestination) {
    Position start{0, 0};
    Position goal{4, 4};
    std::vector<Position> path;
    
    // Create a complete barrier
    for (int x = 0; x < 5; ++x) {
        grid->setCell(Position{x, 2}, CellType::Wall);
    }
    
    EXPECT_FALSE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_TRUE(path.empty());
    EXPECT_FLOAT_EQ(pathfinder->getLastPathCost(), 0.0f);
    EXPECT_GT(pathfinder->getLastSearchSteps(), 0); // Should have attempted search
}

// Test pathfinding from invalid start position
TEST_F(PathfinderTest, InvalidStartPosition) {
    Position start{1, 1};
    Position goal{3, 3};
    std::vector<Position> path;
    
    // Make start position a wall
    grid->setCell(start, CellType::Wall);
    
    EXPECT_FALSE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_TRUE(path.empty());
    EXPECT_FLOAT_EQ(pathfinder->getLastPathCost(), 0.0f);
}

// Test pathfinding to invalid goal position
TEST_F(PathfinderTest, InvalidGoalPosition) {
    Position start{0, 0};
    Position goal{3, 3};
    std::vector<Position> path;
    
    // Make goal position a wall
    grid->setCell(goal, CellType::Wall);
    
    EXPECT_FALSE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_TRUE(path.empty());
    EXPECT_FLOAT_EQ(pathfinder->getLastPathCost(), 0.0f);
}

// Test pathfinding with out of bounds start
TEST_F(PathfinderTest, OutOfBoundsStart) {
    Position start{-1, 0}; // Out of bounds
    Position goal{2, 2};
    std::vector<Position> path;
    
    EXPECT_FALSE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_TRUE(path.empty());
}

// Test pathfinding with out of bounds goal
TEST_F(PathfinderTest, OutOfBoundsGoal) {
    Position start{0, 0};
    Position goal{10, 10}; // Out of bounds
    std::vector<Position> path;
    
    EXPECT_FALSE(pathfinder->findPath(*grid, start, goal, path));
    EXPECT_TRUE(path.empty());
}

// Test path cost accuracy
TEST_F(PathfinderTest, PathCostAccuracy) {
    Position start{0, 0};
    Position goal{3, 0}; // 3 steps horizontally
    std::vector<Position> path;
    
    EXPECT_TRUE(pathfinder->findPath(*grid, start, goal, path));
    
    // For a straight horizontal path, cost should equal distance
    EXPECT_FLOAT_EQ(pathfinder->getLastPathCost(), 3.0f);
    EXPECT_EQ(path.size(), 4); // 4 positions: start + 3 steps
}

// Test search steps counting
TEST_F(PathfinderTest, SearchStepsCount) {
    Position start{0, 0};
    Position goal{1, 1}; // Simple diagonal
    std::vector<Position> path;
    
    EXPECT_TRUE(pathfinder->findPath(*grid, start, goal, path));
    
    // Should have performed some search steps
    EXPECT_GT(pathfinder->getLastSearchSteps(), 0);
    
    // For simple paths, shouldn't need too many steps
    EXPECT_LT(pathfinder->getLastSearchSteps(), 50);
}
} 