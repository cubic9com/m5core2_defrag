#include "GridManager.h"

// Constructor
GridManager::GridManager() {
  initializeRNG();
  initializeGrid();
  initializeRandomGrid();
}

// Initialize random number generator
void GridManager::initializeRNG() {
  // Use a different seed each time
  uint32_t seed = millis() + esp_random(); // Combine ESP32's internal random number generator and millisecond counter
  rng = std::mt19937(seed);
}

// Initialize grid
void GridManager::initializeGrid() {
  grid.resize(Config::Grid::HEIGHT);
  for (int y = 0; y < Config::Grid::HEIGHT; y++) {
    grid[y].reserve(Config::Grid::WIDTH);
    for (int x = 0; x < Config::Grid::WIDTH; x++) {
      grid[y].emplace_back(x, y);
    }
  }
}

// Get drive region type
BlockState GridManager::getDriveRegionState(int y) {
  // Beginning part of the drive (top 25%)
  if (y < Config::Grid::HEIGHT * 0.25) {
    return BlockState::INVISIBLE_UNOPT_BEGIN;
  }
  // Middle part of the drive (25% to 50%)
  else if (y < Config::Grid::HEIGHT * 0.5) {
    return BlockState::INVISIBLE_UNOPT_MIDDLE;
  }
  // First half of the end part of the drive (50% to 70%)
  else if (y < Config::Grid::HEIGHT * 0.7) {
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
  for (int y = 0; y < Config::Grid::HEIGHT; y++) {
    BlockState regionState = getDriveRegionState(y);
    
    for (int x = 0; x < Config::Grid::WIDTH; x++) {
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
