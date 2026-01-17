#include "pathfinding/gridstate.h"

// Constructors
GridState::GridState() : position(0, 0), grid(nullptr) {
}

GridState::GridState(const Position& pos, const Grid* g) : position(pos), grid(g) {
}

GridState::GridState(const GridState& other) : position(other.position), grid(other.grid) {
}

// Assignment operator
GridState& GridState::operator=(const GridState& other) {
    if (this != &other) {
        position = other.position;
        grid = other.grid;
    }
    return *this;
}

// A* interface implementations

// Heuristic function - Manhattan distance to goal
float GridState::GoalDistanceEstimate(GridState& nodeGoal) {
    return static_cast<float>(abs(position.x - nodeGoal.position.x) + 
                             abs(position.y - nodeGoal.position.y));
}

// Check if this is the goal state
bool GridState::IsGoal(GridState& nodeGoal) {
    return position == nodeGoal.position;
}

// Generate successor states (neighboring walkable cells)
bool GridState::GetSuccessors(AStarSearch<GridState>* astarsearch, GridState* parent_node) {
    if (!grid) return false;
    
    // Get all valid neighbors from the grid
    std::vector<Position> neighbors = grid->getNeighbors(position);
    
    for (const Position& neighborPos : neighbors) {
        // Skip the parent position to avoid going backwards
        if (parent_node && neighborPos == parent_node->position) {
            continue;
        }
        
        // Create a new state for this neighbor
        GridState NewNode(neighborPos, grid);
        astarsearch->AddSuccessor(NewNode);
    }
    
    return true;
}

// Cost to move from this state to successor 
float GridState::GetCost(GridState& successor) {
    // For now, all moves have cost 1.0
    return 1.0f;
}

// Check if two states are the same
bool GridState::IsSameState(GridState& rhs) {
    return position == rhs.position;
}

// Hash function for unordered_set
size_t GridState::Hash() {
    // Simple hash combining x and y coordinates
    return static_cast<size_t>(position.x) * 1000 + static_cast<size_t>(position.y);
}