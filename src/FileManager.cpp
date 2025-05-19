#include "FileManager.h"

// Constructor
FileManager::FileManager(GridManager& gridManager)
  : gridManager(gridManager),
    nextFileID(0),
    currentFileToMove(-1),
    isMovingFile(false) {
  assignFileIDs();
}

// Reset
void FileManager::reset() {
  nextFileID = 0;
  currentFileToMove = -1;
  isMovingFile = false;
  assignFileIDs();
}

// Assign file IDs
void FileManager::assignFileIDs() {
  // Use GridManager's random number generator
  std::mt19937& rng = gridManager.getRNG();
  
  // Uniform distribution 0-1 (to determine 50% probability)
  std::uniform_real_distribution<float> continuityDist(0.0f, 1.0f);
  // File ID range (0-400)
  std::uniform_int_distribution<int> fileIdDist(0, 400);
  
  // Previous block's file ID (initial value is -1)
  int previousFileID = -1;
  
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      Block& block = gridManager.getBlock(x, y);
      
      // If previous block is valid, use the same file ID as the previous block with 50% probability
      if (previousFileID >= 0 && continuityDist(rng) < 0.5f) {
        block.fileID = previousFileID;
      } else {
        // Generate a new random file ID
        block.fileID = fileIdDist(rng);
      }
      
      // Save current file ID for the next block
      previousFileID = block.fileID;
    }
  }
}

// Find file to move
void FileManager::findNextFileToMove(int &fileToMove, BlockState &fileType, 
                        std::vector<std::pair<int, int>> &fileBlocks) {
  fileToMove = -1;
  
  // Find unoptimized files
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      Block& block = gridManager.getBlock(x, y);
      if ((block.state == BlockState::UNOPT_BEGIN || 
           block.state == BlockState::UNOPT_MIDDLE || 
           block.state == BlockState::UNOPT_END) && 
          block.fileID >= 0 && 
          !block.isMoving) {
        
        // Found a new file
        fileToMove = block.fileID;
        fileType = block.state;
        
        // Collect all blocks belonging to this file
        collectFileBlocks(fileToMove, fileBlocks);
        break;
      }
    }
    if (fileToMove >= 0) break;
  }
}

// Collect blocks belonging to a file
void FileManager::collectFileBlocks(int fileID, std::vector<std::pair<int, int>> &fileBlocks) {
  fileBlocks.clear();
  
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      Block& block = gridManager.getBlock(x, y);
      if (block.fileID == fileID && 
          (block.state == BlockState::UNOPT_BEGIN || 
           block.state == BlockState::UNOPT_MIDDLE || 
           block.state == BlockState::UNOPT_END)) {
        fileBlocks.push_back(std::make_pair(x, y));
      }
    }
  }
}

// Find target positions for a file
bool FileManager::findTargetPositionsForFile(const std::vector<std::pair<int, int>> &fileBlocks,
                                std::vector<std::pair<int, int>> &targetPositions) {
  targetPositions.clear();
  bool foundTarget = false;
  
  // Search from top-left (in order from the beginning) - horizontal priority
  for (int y = 0; y < gridManager.getRowCount() && !foundTarget; y++) {
    for (int x = 0; x < gridManager.getColumnCount() && !foundTarget; x++) {
      // Check if there are consecutive fileBlocks.size() free spaces from this position
      bool validTarget = true;
      std::vector<std::pair<int, int>> currentTargetPositions;
      
      // Look for consecutive free spaces horizontally
      for (size_t i = 0; i < fileBlocks.size(); i++) {
        int checkX = x + i;
        int checkY = y;
        
        // If the end of the row is reached, move to the next row
        if (checkX >= gridManager.getColumnCount()) {
          checkX = checkX - gridManager.getColumnCount();
          checkY++;
        }
        
        // Check if outside grid range
        if (checkY >= gridManager.getRowCount()) {
          validTarget = false;
          break;
        }
        
        // Check if it's free space - strictly target only free spaces
        if (gridManager.getBlock(checkX, checkY).state != BlockState::FREE) {
          validTarget = false;
          break;
        }
        
        // Add this position to target candidates
        currentTargetPositions.push_back(std::make_pair(checkX, checkY));
      }
      
      if (validTarget && currentTargetPositions.size() == fileBlocks.size()) {
        targetPositions = currentTargetPositions;
        foundTarget = true;
        break; // Use the first valid target found
      }
    }
  }
  
  return foundTarget;
}

// Move file to target
void FileManager::moveFileToTarget(const std::vector<std::pair<int, int>> &fileBlocks,
                      const std::vector<std::pair<int, int>> &targetPositions,
                      int fileID) {
  // Move each block in the file
  for (size_t i = 0; i < fileBlocks.size(); i++) {
    int sourceX = fileBlocks[i].first;
    int sourceY = fileBlocks[i].second;
    int targetX = targetPositions[i].first;
    int targetY = targetPositions[i].second;
    
    // Change the state of the source block to reading
    Block& sourceBlock = gridManager.getBlock(sourceX, sourceY);
    BlockState originalState = sourceBlock.state;
    sourceBlock.state = BlockState::READING;
    
    // Set the target block
    Block& targetBlock = gridManager.getBlock(targetX, targetY);
    
    // Set the target block to writing state
    targetBlock.state = BlockState::WRITING;
    targetBlock.fileID = sourceBlock.fileID;
    
    // Start movement animation (from source to target)
    targetBlock.startMoving(targetX, targetY);
    // Set animation start position to source
    targetBlock.animX = sourceX;
    targetBlock.animY = sourceY;
    
    // Add a slight delay when moving each block to prevent sounds from overlapping
    delay(Config::Animation::BLOCK_MOVE_DELAY);
  }
  
  // Set file moving flag
  isMovingFile = true;
  currentFileToMove = fileID;
}

// Update file movement
void FileManager::updateFileMovement() {
  if (isMovingFile) {
    bool allMoved = true;
    for (int y = 0; y < gridManager.getRowCount(); y++) {
      for (int x = 0; x < gridManager.getColumnCount(); x++) {
        Block& block = gridManager.getBlock(x, y);
        if (block.fileID == currentFileToMove && block.isMoving) {
          allMoved = false;
          break;
        }
      }
      if (!allMoved) break;
    }
    
    if (allMoved) {
      isMovingFile = false;
      currentFileToMove = -1;
    }
  }
}

// Move next file
void FileManager::moveNextFile() {
  // Use GridManager's random number generator
  std::mt19937& rng = gridManager.getRNG();
  
  // Find unoptimized files
  int fileToMove = -1;
  BlockState fileType = BlockState::FREE;
  std::vector<std::pair<int, int>> fileBlocks; // Coordinates of blocks belonging to the file
  
  findNextFileToMove(fileToMove, fileType, fileBlocks);
  
  if (fileToMove < 0 || fileBlocks.empty()) {
    // If there are no unoptimized files, complete
    return;
  }
  
  // Determine the target
  std::vector<std::pair<int, int>> bestTargetPositions;
  bool foundTarget = findTargetPositionsForFile(fileBlocks, bestTargetPositions);
  
  // If a target is found, execute the move process
  if (foundTarget && !bestTargetPositions.empty()) {      
    moveFileToTarget(fileBlocks, bestTargetPositions, fileToMove);
  } else {
    // If no target is found, this file will not be moved
    // Change blocks in the file to optimized (blue)
    for (auto &pos : fileBlocks) {
      gridManager.getBlock(pos.first, pos.second).state = BlockState::OPTIMIZED;
    }
  }
}

// Get whether moving
bool FileManager::isMoving() const {
  return isMovingFile;
}

// Get currently moving file ID
int FileManager::getCurrentFileToMove() const {
  return currentFileToMove;
}

// Get next file ID
int FileManager::getNextFileID() const {
  return nextFileID;
}
