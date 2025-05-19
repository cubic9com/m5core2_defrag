#include "AnimationManager.h"

// Constructor
AnimationManager::AnimationManager(GridManager& gridManager)
  : gridManager(gridManager),
    defragStep(0),
    driveInfoPhase1ScanX(0),
    driveInfoPhase1ScanY(0),
    driveInfoPhase1ScanCompleted(false),
    driveInfoPhase2ScanX(0),
    driveInfoPhase2ScanY(0),
    driveInfoPhase2ScanCompleted(false) {
}

// Reset
void AnimationManager::reset() {
  defragStep = 0;
  driveInfoPhase1ScanX = 0;
  driveInfoPhase1ScanY = 0;
  driveInfoPhase1ScanCompleted = false;
  driveInfoPhase2ScanX = 0;
  driveInfoPhase2ScanY = 0;
  driveInfoPhase2ScanCompleted = false;
}

// Block update process in reading drive info phase 1
void AnimationManager::updateBlocksInDriveInfoPhase1() {
  // Do nothing if scan is complete
  if (driveInfoPhase1ScanCompleted) {
    return;
  }
  
  // Update all blocks in the current row
  for (driveInfoPhase1ScanX = 0; driveInfoPhase1ScanX < gridManager.getColumnCount(); driveInfoPhase1ScanX++) {
    gridManager.getBlock(driveInfoPhase1ScanX, driveInfoPhase1ScanY).updateStateInDriveInfoPhase1();
  }
  
  // Move to the next row
  driveInfoPhase1ScanX = 0;
  driveInfoPhase1ScanY++;
  
  // If all rows have been scanned
  if (driveInfoPhase1ScanY >= gridManager.getRowCount()) {
    driveInfoPhase1ScanCompleted = true;
  }
}

// Block update process in reading drive info phase 2
void AnimationManager::updateBlocksInDriveInfoPhase2() {
  // Do nothing if scan is complete
  if (driveInfoPhase2ScanCompleted) {
    return;
  }
  
  // Update all blocks in the current row
  for (driveInfoPhase2ScanX = 0; driveInfoPhase2ScanX < gridManager.getColumnCount(); driveInfoPhase2ScanX++) {
    gridManager.getBlock(driveInfoPhase2ScanX, driveInfoPhase2ScanY).updateStateInDriveInfoPhase2();
  }
  
  // Move to the next row
  driveInfoPhase2ScanX = 0;
  driveInfoPhase2ScanY++;
  
  // If all rows have been scanned
  if (driveInfoPhase2ScanY >= gridManager.getRowCount()) {
    driveInfoPhase2ScanCompleted = true;
  }
}

// Collect positions of moving and reading blocks
void AnimationManager::collectMovingAndReadingBlocks(std::vector<std::pair<int, int>>& movingBlockPositions, 
                                                   std::vector<std::pair<int, int>>& readingBlockPositions) {
  movingBlockPositions.clear();
  readingBlockPositions.clear();
  
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      Block& block = gridManager.getBlock(x, y);
      if (block.isMoving) {
        // Record the position of moving blocks
        movingBlockPositions.push_back(std::make_pair(x, y));
      } else if (block.state == BlockState::READING) {
        // Record the position of blocks being read (source blocks)
        readingBlockPositions.push_back(std::make_pair(x, y));
      }
    }
  }
}

// Update all blocks
void AnimationManager::updateAllBlocks() {
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      gridManager.getBlock(x, y).updateMoving();
    }
  }
}

// Process blocks that have completed movement
bool AnimationManager::processCompletedBlocks(const std::vector<std::pair<int, int>>& movingBlockPositions, 
                                            const std::vector<std::pair<int, int>>& readingBlockPositions) {
  bool anyBlockCompleted = false;
  
  // Process blocks that have completed movement
  for (auto &pos : movingBlockPositions) {
    int x = pos.first;
    int y = pos.second;
    Block& block = gridManager.getBlock(x, y);
    
    // Check if movement is complete (when isMoving flag becomes false)
    if (!block.isMoving) {
      anyBlockCompleted = true;
      
      // If the target position and current position are the same (when movement is complete)
      if (x == block.targetX && y == block.targetY) {
        // Set the target block to optimized
        block.state = BlockState::OPTIMIZED;
      }
    }
  }
  
  // If movement is complete, set the source blocks to free space
  if (anyBlockCompleted) {
    for (auto &pos : readingBlockPositions) {
      int x = pos.first;
      int y = pos.second;
      Block& block = gridManager.getBlock(x, y);
      
      // Set the source block to free space
      block.state = BlockState::FREE;
      block.fileID = -1;
    }
  }
  
  return anyBlockCompleted;
}

// Block update process in defragmenting
void AnimationManager::updateBlocksInDefragmenting() {
  // Record positions of moving blocks
  std::vector<std::pair<int, int>> movingBlockPositions;
  // Record positions of blocks being read (READING)
  std::vector<std::pair<int, int>> readingBlockPositions;
  
  // Collect positions of moving and reading blocks
  collectMovingAndReadingBlocks(movingBlockPositions, readingBlockPositions);
  
  // Update all blocks
  updateAllBlocks();
  
  // Process blocks that have completed movement
  processCompletedBlocks(movingBlockPositions, readingBlockPositions);
}

// Update defrag step
void AnimationManager::incrementDefragStep() {
  defragStep++;
}

// Get defrag step
int AnimationManager::getDefragStep() const {
  return defragStep;
}

// Count the number of unoptimized blocks
int AnimationManager::countUnoptimizedBlocks() const {
  int unoptimizedCount = 0;
  
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      const Block& block = gridManager.getBlock(x, y);
      switch (block.state) {
        // Don't count optimized or free space blocks
        case BlockState::FREE:
        case BlockState::FIXED:
        case BlockState::OPTIMIZED:
        case BlockState::READING:
          break;
        
        // Count other blocks (unoptimized)
        default:
          unoptimizedCount++;
          break;
      }
    }
  }
  
  return unoptimizedCount;
}

// Count the total number of blocks (excluding free space)
int AnimationManager::countTotalBlocks() const {
  int totalBlocks = 0;
  
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      const Block& block = gridManager.getBlock(x, y);
      if (block.state != BlockState::FREE) {
        totalBlocks++;
      }
    }
  }
  
  return totalBlocks;
}

// Calculate defragmentation progress in defragmenting (percentage)
int AnimationManager::calculateDefragmentingCompletionPercentage() const {
  int totalBlocks = countTotalBlocks();
  int unoptimizedBlocks = countUnoptimizedBlocks();
  
  if (totalBlocks > 0) {
    int optimizedBlocks = totalBlocks - unoptimizedBlocks;
    return (optimizedBlocks * 100) / totalBlocks;
  } else {
    return 100; // If total blocks is 0, consider it 100% complete
  }
}

// Calculate progress in reading drive information reading phase 1 (percentage)
int AnimationManager::calculateDriveInfoPhase1CompletionPercentage() const {
  int totalCells = gridManager.getRowCount() * gridManager.getColumnCount();
  int scannedCells = driveInfoPhase1ScanY * gridManager.getColumnCount() + driveInfoPhase1ScanX;
  
  if (totalCells > 0) {
    return (scannedCells * 100) / totalCells;
  } else {
    return 100; // If total cells is 0, consider it 100% complete
  }
}

// Calculate progress in reading drive information reading phase 2 (percentage)
int AnimationManager::calculateDriveInfoPhase2CompletionPercentage() const {
  int totalCells = gridManager.getRowCount() * gridManager.getColumnCount();
  int scannedCells = driveInfoPhase2ScanY * gridManager.getColumnCount() + driveInfoPhase2ScanX;
  
  if (totalCells > 0) {
    return (scannedCells * 100) / totalCells;
  } else {
    return 100; // If total cells is 0, consider it 100% complete
  }
}

// Calculate total progress in reading drive information reading phases 1 and 2 (percentage)
int AnimationManager::calculateDriveInfoPhasesCompletionPercentage() const {
  // Calculate progress with phase 1 and phase 2 each accounting for 50%
  if (driveInfoPhase1ScanCompleted) {
    // If phase 1 is complete, calculate phase 2 progress as 50-100%
    return 50 + (calculateDriveInfoPhase2CompletionPercentage() / 2);
  } else {
    // If phase 1 is not complete, calculate phase 1 progress as 0-50%
    return calculateDriveInfoPhase1CompletionPercentage() / 2;
  }
}

// Get whether reading drive information phase 1 scan is complete
bool AnimationManager::isDriveInfoPhase1ScanCompleted() const {
  return driveInfoPhase1ScanCompleted;
}

// Get whether reading drive information phase 2 scan is complete
bool AnimationManager::isDriveInfoPhase2ScanCompleted() const {
  return driveInfoPhase2ScanCompleted;
}

// Get current phase 1 scan position
int AnimationManager::getDriveInfoPhase1ScanX() const {
  return driveInfoPhase1ScanX;
}

int AnimationManager::getDriveInfoPhase1ScanY() const {
  return driveInfoPhase1ScanY;
}

// Get current phase 2 scan position
int AnimationManager::getDriveInfoPhase2ScanX() const {
  return driveInfoPhase2ScanX;
}

int AnimationManager::getDriveInfoPhase2ScanY() const {
  return driveInfoPhase2ScanY;
}
