#include "pathfinding/pathfinder.h"
#include <iostream>

// Constructor
Pathfinder::Pathfinder() : lastPathCost_(0.0f), lastSearchSteps_(0) {
}

// Destructor
Pathfinder::~Pathfinder() {
    astarsearch_.EnsureMemoryFreed();
}

bool Pathfinder::findPath(const Grid& grid, const Position& start, const Position& goal, std::vector<Position>& path) {

    path.clear();
    lastPathCost_ = 0.0f;
    lastSearchSteps_ = 0;
    
    // Validate start and goal positions
    if (!grid.isInBounds(start) || !grid.isWalkable(start)) {
        std::cerr << "Start position is not valid or walkable!" << std::endl;
        return false;
    }
    
    if (!grid.isInBounds(goal) || !grid.isWalkable(goal)) {
        std::cerr << "Goal position is not valid or walkable!" << std::endl;
        return false;
    }
    
    // Create start and goal states
    GridState nodeStart(start, &grid);
    GridState nodeEnd(goal, &grid);
    
    // Set A* start and goal states
    astarsearch_.SetStartAndGoalStates(nodeStart, nodeEnd);
    
    unsigned int SearchState;
    unsigned int SearchSteps = 0;
    
    // Perform the search step by step until complete
    do {
        SearchState = astarsearch_.SearchStep();
        SearchSteps++;
        
    } while (SearchState == AStarSearch<GridState>::SEARCH_STATE_SEARCHING);
    
    lastSearchSteps_ = SearchSteps;
    
    if (SearchState == AStarSearch<GridState>::SEARCH_STATE_SUCCEEDED) {
        // Found a path! Extract it
        std::cout << "Path found in " << SearchSteps << " steps!" << std::endl;
        
        GridState* node = astarsearch_.GetSolutionStart();
        
        // Build the path
        while (true) {
            path.push_back(node->position);
            
            node = astarsearch_.GetSolutionNext();
            
            if (!node) {
                break;
            }
        }
        
        // Get path cost
        lastPathCost_ = astarsearch_.GetSolutionCost();
        
        // Clean up memory
        astarsearch_.FreeSolutionNodes();
        
        std::cout << "Path cost: " << lastPathCost_ << std::endl;
        std::cout << "Path length: " << path.size() << " steps" << std::endl;
        
        return true;
        
    } else if (SearchState == AStarSearch<GridState>::SEARCH_STATE_FAILED) {
        std::cout << "Search terminated. No solution found." << std::endl;
        return false;
        
    } else if (SearchState == AStarSearch<GridState>::SEARCH_STATE_OUT_OF_MEMORY) {
        std::cout << "Search terminated. Out of memory." << std::endl;
        return false;
    }
    
    return false;
}