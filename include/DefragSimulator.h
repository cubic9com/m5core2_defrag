/**
 * @file DefragSimulator.h
 * @brief Main controller for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the DefragSimulator class which orchestrates the entire
 * disk defragmentation animation, managing all subsystems including grid,
 * file management, animations, UI rendering, and sound effects.
 */

#pragma once

#include <M5Unified.h>
#include <M5GFX.h>
#include "Config.h"
#include "Colors.h"
#include "Enums.h"
#include "GridManager.h"
#include "FileManager.h"
#include "AnimationManager.h"
#include "UIRenderer.h"
#include "SoundManager.h"

// Defrag simulator class
class DefragSimulator {
private:
  GridManager gridManager;
  FileManager fileManager;
  AnimationManager animationManager;
  UIRenderer uiRenderer;
  SoundManager soundManager;
  
  // Record the time when completed state is reached
  unsigned long completedStateStartTime;
  bool isInCompletedState;
  
  // Touch detection related
  bool isInTouchState;
  unsigned long touchStateStartTime;
  int touchX, touchY;  // Touched coordinates (grid coordinates)
  
public:
  // Constructor
  DefragSimulator();

  // Initialization
  void initialize();

  // Reset
  void reset();
  
  // Update process
  void update();
  
  // Draw grid and UI elements
  void draw();
  
  // Get state
  AnimationState getState() const;
  
  // Set state
  void setState(AnimationState newState);
  
  // Process when touch is detected
  void handleTouch(int x, int y);
  
  // Explode all blocks
  void setAllBlocksToBAD(int touchX, int touchY);
};

// Global instance (external declaration)
extern DefragSimulator defragSim;
