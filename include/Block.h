/**
 * @file Block.h
 * @brief Block representation for disk defragmentation visualization
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file defines the Block class which represents individual blocks
 * in the disk defragmentation animation, including their states, positions,
 * and animation properties.
 */

#pragma once

#include <M5GFX.h>
#include "Config.h"
#include "Colors.h"
#include "Enums.h"

// Canvas for off-screen rendering (external declaration)
extern M5Canvas canvas;

// Block class
class Block {
public:
  BlockState state;
  int x, y;  // Position on the grid
  int fileID;  // ID of blocks belonging to the same file
  int targetX, targetY;  // Target position (for animation)
  float animX, animY; // Floating-point coordinates for animation
  bool isMoving;  // Whether it's moving
  bool isExploding;  // Whether it's exploding
  float explodeVelocityX, explodeVelocityY; // Explosion velocity vector
  
  // Constructor
  Block(int _x, int _y);
  
  // Get the color based on the block's state
  uint16_t getColor() const;
  
  // Draw the block
  void draw(AnimationState animState);
  
  // Update the block's state in reading drive info phase 1
  void updateStateInDriveInfoPhase1();

  // Update the block's state in reading drive info phase 2
  void updateStateInDriveInfoPhase2();

  // Start moving
  void startMoving(int newX, int newY);
  
  // Update moving animation in defragmenting
  void updateMoving();
  
  // Start explosion (from center coordinates)
  void startExploding(int centerX, int centerY);
  
  // Update explosion animation
  void updateExplosion();
};
