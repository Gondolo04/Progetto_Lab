#pragma once
#include "grid.h"
#include "gridstate.h"
#include "stlastar.h"
#include <vector>

class Pathfinder {
public:
    Pathfinder();
    ~Pathfinder();
    
    // Find path from start to goal position using A* algorithm
    // Returns true if path found, false otherwise
    bool findPath(const Grid& grid, const Position& start, const Position& goal, 
                  std::vector<Position>& path);
    
    // Get the cost of the last found path
    float getLastPathCost() const { return lastPathCost_; }
    
    // Get number of search steps for the last path
    int getLastSearchSteps() const { return lastSearchSteps_; }

private:
    AStarSearch<GridState> astarsearch_;
    float lastPathCost_;
    int lastSearchSteps_;
};