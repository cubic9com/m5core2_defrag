#pragma once

#include <vector>
#include <random>
#include "Config.h"
#include "Colors.h"
#include "Enums.h"
#include "Block.h"

// Grid management class
class GridManager {
private:
  std::vector<std::vector<Block>> grid;
  std::mt19937 rng;
  
  // Initialize random number generator
  void initializeRNG();
  
public:
  // Constructor
  GridManager();
  
  // Initialize grid
  void initializeGrid();
  
  // Randomly set initial block states
  void initializeRandomGrid();
  
  // Get drive region type
  BlockState getDriveRegionState(int y);
  
  // Set initial block state
  void setInitialBlockState(int x, int y, int randomValue, BlockState primaryState);
  
  // Access to grid
  Block& getBlock(int x, int y);
  const Block& getBlock(int x, int y) const;
  
  // Get row and column count of grid
  int getRowCount() const;
  int getColumnCount() const;
  
  // Get grid row
  std::vector<Block>& getRow(int y);
  const std::vector<Block>& getRow(int y) const;
  
  // Access to random number generator
  std::mt19937& getRNG();
};
