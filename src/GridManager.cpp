/**
 * @file GridManager.cpp
 * @brief Implementation of grid management for disk defragmentation visualization
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the GridManager class which manages the grid of blocks
 * representing disk sectors in the defragmentation simulation.
 */

#include "GridManager.h"
#include "PlatformCompat.h"

// Constructor
GridManager::GridManager() {
  initializeRNG();
  initializeGrid();
  initializeRandomGrid();
}

// Initialize random number generator
void GridManager::initializeRNG() {
  // Use a different seed each time
  uint32_t seed = seed();
  rng = std::mt19937(seed);
}

// Initialize grid
void GridManager::initializeGrid() {
  grid.resize(Config::getGridRows());
  for (int y = 0; y < Config::getGridRows(); y++) {
    grid[y].reserve(Config::getGridCols());
    for (int x = 0; x < Config::getGridCols(); x++) {
      grid[y].emplace_back(x, y);
    }
  }
}

// Get drive region type
BlockState GridManager::getDriveRegionState(int y) {
  // Beginning part of the drive (top 25%)
  if (y < Config::getGridRows() * 0.25) {
    return BlockState::INVISIBLE_UNOPT_BEGIN;
  }
  // Middle part of the drive (25% to 50%)
  else if (y < Config::getGridRows() * 0.5) {
    return BlockState::INVISIBLE_UNOPT_MIDDLE;
  }
  // First half of the end part of the drive (50% to 70%)
  else if (y < Config::getGridRows() * 0.7) {
    return BlockState::INVISIBLE_UNOPT_END;
  }
  // Second half of the end part of the drive (bottom 30%)
  else {
    return BlockState::INVISIBLE_FREE;
  }
}

// Randomly set initial block states
void GridManager::initializeRandomGrid() {
  std::uniform_int_distribution<int> dist(0, 100);
  
  // Place different types of blocks according to drive position
  for (int y = 0; y < Config::getGridRows(); y++) {
    BlockState regionState = getDriveRegionState(y);
    
    for (int x = 0; x < Config::getGridCols(); x++) {
      int r = dist(rng);
      
      // The second half of the end part is all free space
      if (regionState == BlockState::INVISIBLE_FREE) {
        grid[y][x].state = BlockState::INVISIBLE_FREE;
      } else {
        setInitialBlockState(x, y, r, regionState);
      }
    }
  }
}

// Set initial block state
void GridManager::setInitialBlockState(int x, int y, int randomValue, BlockState primaryState) {
  if (randomValue < 60) { // 60% probability for primaryState
    grid[y][x].state = primaryState;
  } else if (randomValue < 80) { // 20% probability for optimized
    grid[y][x].state = BlockState::INVISIBLE_OPTIMIZED;
  } else if (randomValue < 85) { // 5% probability for fixed data
    // Set fixed data state corresponding to primaryState
    switch (primaryState) {
      case BlockState::INVISIBLE_UNOPT_BEGIN:
        grid[y][x].state = BlockState::INVISIBLE_FIXED_AS_UNOPT_BEGIN;
        break;
      case BlockState::INVISIBLE_UNOPT_MIDDLE:
        grid[y][x].state = BlockState::INVISIBLE_FIXED_AS_UNOPT_MIDDLE;
        break;
      case BlockState::INVISIBLE_UNOPT_END:
        grid[y][x].state = BlockState::INVISIBLE_FIXED_AS_UNOPT_END;
        break;
      default:
        grid[y][x].state = BlockState::INVISIBLE_FREE;
        break;
    }
  } else { // 15% probability for free space
    grid[y][x].state = BlockState::INVISIBLE_FREE;
  }
}

// Access to grid
Block& GridManager::getBlock(int x, int y) {
  return grid[y][x];
}

const Block& GridManager::getBlock(int x, int y) const {
  return grid[y][x];
}

// Get row and column count of grid
int GridManager::getRowCount() const {
  return grid.size();
}

int GridManager::getColumnCount() const {
  return grid.empty() ? 0 : grid[0].size();
}

// Get grid row
std::vector<Block>& GridManager::getRow(int y) {
  return grid[y];
}

const std::vector<Block>& GridManager::getRow(int y) const {
  return grid[y];
}

// Access to random number generator
std::mt19937& GridManager::getRNG() {
  return rng;
}
