/**
 * @file FileManager.h
 * @brief File management for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the FileManager class which handles file identification,
 * grouping, and movement operations during the defragmentation process.
 */

#pragma once

#include <vector>
#include <random>
#include <utility>
#include "Config.h"
#include "Enums.h"
#include "GridManager.h"

// File management class
class FileManager {
private:
  GridManager& gridManager;
  int nextFileID; // Next file ID to assign
  int currentFileToMove; // Currently moving file ID
  bool isMovingFile; // File moving flag

  public:
  // Constructor
  FileManager(GridManager& gridManager);
  
  // Reset
  void reset();

  // Assign file IDs
  void assignFileIDs();
  
  // Find file to move
  void findNextFileToMove(int &fileToMove, BlockState &fileType, 
                            std::vector<std::pair<int, int>> &fileBlocks);
  
  // Collect blocks belonging to a file
  void collectFileBlocks(int fileID, std::vector<std::pair<int, int>> &fileBlocks);
  
  // Find target positions for a file
  bool findTargetPositionsForFile(const std::vector<std::pair<int, int>> &fileBlocks,
                                    std::vector<std::pair<int, int>> &targetPositions);
  
  // Move file to target
  void moveFileToTarget(const std::vector<std::pair<int, int>> &fileBlocks,
                          const std::vector<std::pair<int, int>> &targetPositions,
                          int fileID);
  
  // Update file movement
  void updateFileMovement();
  
  // Move next file
  void moveNextFile();
  
  // Get whether moving
  bool isMoving() const;
  
  // Get currently moving file ID
  int getCurrentFileToMove() const;
  
  // Get next file ID
  int getNextFileID() const;
};
