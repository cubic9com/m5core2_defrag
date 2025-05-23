/**
 * @file AnimationManager.h
 * @brief Animation management for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the AnimationManager class which handles all animation
 * states and transitions for the disk defragmentation visualization on M5Stack.
 */

#pragma once

#include <vector>
#include <utility>
#include "Config.h"
#include "Enums.h"
#include "GridManager.h"

// Animation management class
class AnimationManager {
private:
  GridManager& gridManager;
  int defragStep;
  int driveInfoPhase1ScanX;  // X-coordinate during scanning in reading drive info phase 1
  int driveInfoPhase1ScanY;  // Y-coordinate during scanning in reading drive info phase 1
  bool driveInfoPhase1ScanCompleted;  // Whether the scan is complete in reading drive info phase 1
  int driveInfoPhase2ScanX;  // X-coordinate during scanning in reading drive info phase 2
  int driveInfoPhase2ScanY;  // Y-coordinate during scanning in reading drive info phase 2
  bool driveInfoPhase2ScanCompleted;  // Whether the scan is complete in reading drive info phase 2
 
  // Collect positions of moving and reading blocks
  void collectMovingAndReadingBlocks(std::vector<std::pair<int, int>>& movingBlockPositions, 
                                    std::vector<std::pair<int, int>>& readingBlockPositions);
  
  // Update all blocks
  void updateAllBlocks();
  
  // Process blocks that have completed movement
  bool processCompletedBlocks(const std::vector<std::pair<int, int>>& movingBlockPositions, 
                             const std::vector<std::pair<int, int>>& readingBlockPositions);
  
public:
  // Constructor
  AnimationManager(GridManager& gridManager);
  
  // Reset
  void reset();
  
  // Block update process in reading drive info phase 1
  void updateBlocksInDriveInfoPhase1();

  // Block update process in reading drive info phase 2
  void updateBlocksInDriveInfoPhase2();

  // Block update process in defragmenting
  void updateBlocksInDefragmenting();
  
  // Update defrag step
  void incrementDefragStep();
  
  // Get defrag step
  int getDefragStep() const;
  
  // Count the number of unoptimized blocks
  int countUnoptimizedBlocks() const;
  
  // Count the total number of blocks (excluding free space)
  int countTotalBlocks() const;
  
  // Calculate progress in defragmenting (percentage)
  int calculateDefragmentingCompletionPercentage() const;
  
  // Calculate progress of reading drive info phase 1 (percentage)
  int calculateDriveInfoPhase1CompletionPercentage() const;
  
  // Calculate progress of reading drive info phase 1 (percentage)
  int calculateDriveInfoPhase2CompletionPercentage() const;
  
  // Calculate total progress of reading drive info phase 1 and phase 2 (percentage)
  int calculateDriveInfoPhasesCompletionPercentage() const;
  
  // Get whether the scan is complete
  bool isDriveInfoPhase1ScanCompleted() const;
  
  // Get whether the Phase2 scan is complete
  bool isDriveInfoPhase2ScanCompleted() const;
  
  // Get current scan position
  int getDriveInfoPhase1ScanX() const;
  int getDriveInfoPhase1ScanY() const;
  
  // Get current Phase2 scan position
  int getDriveInfoPhase2ScanX() const;
  int getDriveInfoPhase2ScanY() const;
};
