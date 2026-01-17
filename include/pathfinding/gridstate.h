#pragma once
#include "grid.h"
#include "stlastar.h"
#include <cmath>

// GridState represents a position on the grid for A* pathfinding
class GridState : public AStarState<GridState> {
public:
    Position position;
    const Grid* grid;  // Reference to the grid for validation
    
    // Constructors
    GridState();
    GridState(const Position& pos, const Grid* g);
    GridState(const GridState& other);
    
    // Assignment operator
    GridState& operator=(const GridState& other);

    // A* interface implementations
    float GoalDistanceEstimate(GridState& nodeGoal) override;
    bool IsGoal(GridState& nodeGoal) override;
    bool GetSuccessors(AStarSearch<GridState>* astarsearch, GridState* parent_node) override;
    float GetCost(GridState& successor) override;
    bool IsSameState(GridState& rhs) override;
    size_t Hash() override;
};