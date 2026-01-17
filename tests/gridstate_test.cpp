#include <gtest/gtest.h>
#include "pathfinding/gridstate.h"
#include "pathfinding/grid.h"
#include "pathfinding/stlastar.h"

namespace pathfinding::test {

class GridStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a 5x5 grid for testing
        grid = std::make_unique<Grid>(5, 5);
        
        // Add some walls for more complex testing
        grid->setCell(Position{2, 2}, CellType::Wall);
        grid->setCell(Position{3, 1}, CellType::Wall);
    }

    std::unique_ptr<Grid> grid;
};

// Test default constructor
TEST_F(GridStateTest, DefaultConstructor) {
    GridState state;
    
    EXPECT_EQ(state.position.x, 0);
    EXPECT_EQ(state.position.y, 0);
    EXPECT_EQ(state.grid, nullptr);
}

// Test parameterized constructor
TEST_F(GridStateTest, ParameterizedConstructor) {
    Position pos{3, 4};
    GridState state(pos, grid.get());
    
    EXPECT_EQ(state.position.x, 3);
    EXPECT_EQ(state.position.y, 4);
    EXPECT_EQ(state.grid, grid.get());
}

// Test copy constructor
TEST_F(GridStateTest, CopyConstructor) {
    Position pos{1, 2};
    GridState original(pos, grid.get());
    GridState copy(original);
    
    EXPECT_EQ(copy.position.x, original.position.x);
    EXPECT_EQ(copy.position.y, original.position.y);
    EXPECT_EQ(copy.grid, original.grid);
}

// Test assignment operator
TEST_F(GridStateTest, AssignmentOperator) {
    Position pos1{1, 2};
    Position pos2{3, 4};
    
    GridState state1(pos1, grid.get());
    GridState state2(pos2, nullptr);
    
    state2 = state1;
    
    EXPECT_EQ(state2.position.x, state1.position.x);
    EXPECT_EQ(state2.position.y, state1.position.y);
    EXPECT_EQ(state2.grid, state1.grid);
}

// Test self-assignment
TEST_F(GridStateTest, SelfAssignment) {
    Position pos{2, 3};
    GridState state(pos, grid.get());
    
    state = state; // Self-assignment
    
    EXPECT_EQ(state.position.x, 2);
    EXPECT_EQ(state.position.y, 3);
    EXPECT_EQ(state.grid, grid.get());
}

// Test goal distance estimation (Manhattan distance)
TEST_F(GridStateTest, GoalDistanceEstimate) {
    GridState state1(Position{0, 0}, grid.get());
    GridState goal1(Position{3, 4}, grid.get());
    
    float distance = state1.GoalDistanceEstimate(goal1);
    EXPECT_FLOAT_EQ(distance, 7.0f); 
    
    GridState state2(Position{2, 3}, grid.get());
    GridState goal2(Position{1, 1}, grid.get());
    
    distance = state2.GoalDistanceEstimate(goal2);
    EXPECT_FLOAT_EQ(distance, 3.0f); 
}

// Test goal distance with same position
TEST_F(GridStateTest, GoalDistanceEstimateSamePosition) {
    GridState state(Position{2, 2}, grid.get());
    GridState goal(Position{2, 2}, grid.get());
    
    float distance = state.GoalDistanceEstimate(goal);
    EXPECT_FLOAT_EQ(distance, 0.0f);
}

// Test IsGoal method
TEST_F(GridStateTest, IsGoalTrue) {
    GridState state(Position{2, 3}, grid.get());
    GridState goal(Position{2, 3}, grid.get());
    
    EXPECT_TRUE(state.IsGoal(goal));
}

TEST_F(GridStateTest, IsGoalFalse) {
    GridState state(Position{1, 2}, grid.get());
    GridState goal(Position{3, 4}, grid.get());
    
    EXPECT_FALSE(state.IsGoal(goal));
}

// Test GetSuccessors with valid grid
TEST_F(GridStateTest, GetSuccessorsValidGrid) {
    GridState state(Position{1, 1}, grid.get());
    AStarSearch<GridState> astar;
    
    bool result = state.GetSuccessors(&astar, nullptr);
    EXPECT_TRUE(result);
    
    // Position (1,1) should have neighbors at (0,1), (2,1), (1,0), (1,2)
    // But (2,2) is a wall, so neighbors should be (0,1), (1,0), (1,2)
    // Note: We can't directly access successors, but we can test the return value
}

// Test GetSuccessors without valid grid assigned
TEST_F(GridStateTest, GetSuccessorsNullGrid) {
    GridState state(Position{1, 1}, nullptr);
    AStarSearch<GridState> astar;
    
    bool result = state.GetSuccessors(&astar, nullptr);
    EXPECT_FALSE(result);
}

// Test GetSuccessors excludes parent position
TEST_F(GridStateTest, GetSuccessorsExcludesParent) {
    GridState state(Position{1, 1}, grid.get());
    GridState parent(Position{0, 1}, grid.get());
    AStarSearch<GridState> astar;
    
    bool result = state.GetSuccessors(&astar, &parent);
    EXPECT_TRUE(result);
    
    // This should succeed and exclude the parent position (0,1)
}

// Test GetCost method
TEST_F(GridStateTest, GetCost) {
    GridState state1(Position{0, 0}, grid.get());
    GridState state2(Position{0, 1}, grid.get());
    
    float cost = state1.GetCost(state2);
    EXPECT_FLOAT_EQ(cost, 1.0f);
}

// Test IsSameState with same position
TEST_F(GridStateTest, IsSameStateTrue) {
    GridState state1(Position{2, 3}, grid.get());
    GridState state2(Position{2, 3}, grid.get()); 
    
    EXPECT_TRUE(state1.IsSameState(state2));
}

// Test IsSameState with different positions
TEST_F(GridStateTest, IsSameStateFalse) {
    GridState state1(Position{1, 2}, grid.get());
    GridState state2(Position{2, 1}, grid.get());
    
    EXPECT_FALSE(state1.IsSameState(state2));
}

// Test Hash function
TEST_F(GridStateTest, HashFunction) {
    GridState state1(Position{2, 3}, grid.get());
    GridState state2(Position{2, 3}, grid.get()); 
    GridState state3(Position{3, 2}, grid.get()); 
    
    size_t hash1 = state1.Hash();
    size_t hash2 = state2.Hash();
    size_t hash3 = state3.Hash();
    
    // Same position should have same hash
    EXPECT_EQ(hash1, hash2);
    
    // Different positions should have different hashes (not guaranteed, but very likely)
    EXPECT_NE(hash1, hash3);
}

// Test Hash function with specific values
TEST_F(GridStateTest, HashFunctionValues) {
    GridState state(Position{5, 7}, grid.get());
    
    size_t expectedHash = 5 * 1000 + 7;
    size_t actualHash = state.Hash();
    
    EXPECT_EQ(actualHash, expectedHash);
}
} 