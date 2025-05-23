/**
 * @file UIRenderer.h
 * @brief User interface rendering for disk defragmentation visualization
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the UIRenderer class which handles all user interface
 * rendering including the window frame, progress bar, grid display, and
 * hit counter for the disk defragmentation animation.
 */

#pragma once

#include <M5GFX.h>
#include "Config.h"
#include "Colors.h"
#include "Enums.h"
#include "GridManager.h"
#include "AnimationManager.h"

// External declaration
extern M5Canvas canvas;

// UI rendering class
class UIRenderer {
private:
  AnimationState state;
  int completionPercentage;
  int screenWidth;  // Member variable to store screen width
  int screenHeight;  // Member variable to store screen height
  int hitCounter; // Hit counter
  
public:
  // Constructor
  UIRenderer();
  
  // Initialization
  void initialize();

  // Draw UI elements
  void drawUI();
  
  // Draw hit counter
  void drawHitCounter();

  // Draw grid and UI elements
  void draw(GridManager& gridManager, AnimationManager& animationManager);
  
  // Set state
  void setState(AnimationState newState);
  
  // Get state
  AnimationState getState() const;
  
  // Set completion percentage
  void setCompletionPercentage(int percentage);
  
  // Get completion percentage
  int getCompletionPercentage() const;
  
  // Increment hit counter
  void incrementHitCounter();
  
  // Get hit counter
  int getHitCounter() const;
};
