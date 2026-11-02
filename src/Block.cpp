/**
 * @file Block.cpp
 * @brief Implementation of block representation for disk defragmentation visualization
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the Block class which represents individual blocks
 * in the disk defragmentation animation, including their states, positions,
 * animations, and rendering logic.
 */

#include "Block.h"

Block::Block(int _x, int _y) : 
  x(_x), y(_y), 
  targetX(_x), targetY(_y), 
  animX(_x), animY(_y),
  state(BlockState::FREE), 
  isMoving(false), 
  fileID(-1),
  isExploding(false),
  explodeVelocityX(0.0f),
  explodeVelocityY(0.0f) {}

// Get the color based on the block's state
uint16_t Block::getColor() const {
  switch (state) {
    case BlockState::FIXED:
      return Colors::Block::FIXED_BACK;
    case BlockState::UNOPT_BEGIN:
    case BlockState::FIXED_AS_UNOPT_BEGIN:
      return Colors::Block::UNOPT_BEGIN;
    case BlockState::UNOPT_MIDDLE:
    case BlockState::FIXED_AS_UNOPT_MIDDLE:
      return Colors::Block::UNOPT_MIDDLE;
    case BlockState::UNOPT_END:
    case BlockState::FIXED_AS_UNOPT_END:
      return Colors::Block::UNOPT_END;
    case BlockState::OPTIMIZED:
      return Colors::Block::OPTIMIZED;
    case BlockState::READING:
      return Colors::Block::READING;
    case BlockState::WRITING:
      return Colors::Block::WRITING;
    case BlockState::BAD:
      return Colors::Block::BAD_BACK;
    default:
      return Colors::Block::FREE;
  }
}

// Draw the block
void Block::draw(AnimationState animState) {
  // Use floating-point coordinates during animation (movement or explosion)
  float useX = (isMoving || isExploding) ? animX : x;
  float useY = (isMoving || isExploding) ? animY : y;
  
  int screenX = Config::getGridOffsetX() + useX * (Config::getBlockWidth() + 2);
  int screenY = Config::getGridOffsetY() + useY * (Config::getBlockHeight() + 2);
  
  switch (state) {
    // Blocks that are not drawn
    case BlockState::INVISIBLE_FREE:
    case BlockState::INVISIBLE_FIXED_AS_UNOPT_BEGIN:
    case BlockState::INVISIBLE_FIXED_AS_UNOPT_MIDDLE:
    case BlockState::INVISIBLE_FIXED_AS_UNOPT_END:
    case BlockState::INVISIBLE_UNOPT_BEGIN:
    case BlockState::INVISIBLE_UNOPT_MIDDLE:
    case BlockState::INVISIBLE_UNOPT_END:
    case BlockState::INVISIBLE_OPTIMIZED:
    case BlockState::FREE:
      break;
      
    // Special drawing for immovable data blocks
    case BlockState::FIXED:
      {
        uint16_t color = getColor();
        canvas.fillRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), color);
        canvas.drawRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), Colors::Block::FRAME);
        
        // Special drawing for immovable data (red square in the upper right)
        canvas.fillRect(screenX + Config::getBlockWidth() / 2, screenY, 
                      Config::getBlockWidth() / 2, Config::getBlockHeight() / 2, Colors::Block::FIXED_FORE);
        canvas.drawRect(screenX + Config::getBlockWidth() / 2, screenY, 
                      Config::getBlockWidth() / 2, Config::getBlockHeight() / 2, Colors::Block::FRAME);
      }
      break;
      
    // Special drawing for bad blocks
    case BlockState::BAD:
      {
        uint16_t color = getColor();
        canvas.fillRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), color);
        canvas.drawRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), Colors::Block::FRAME);
        
        // Special drawing for bad blocks (diagonal line)
        canvas.drawLine(screenX + 1, screenY + 1, 
                      screenX + Config::getBlockWidth() - 2, screenY + Config::getBlockHeight() - 2, Colors::Block::BAD_FORE);
      }
      break;
      
    // Other blocks to display
    default:
      {
        uint16_t color = getColor();
        canvas.fillRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), color);
        canvas.drawRect(screenX, screenY, Config::getBlockWidth(), Config::getBlockHeight(), Colors::Block::FRAME);
      }
      break;
  }
}

// Update the block's state in reading drive info phase 1
void Block::updateStateInDriveInfoPhase1() {
    switch (state) {
        case BlockState::INVISIBLE_FREE:
            state = BlockState::FREE;
            break;
        case BlockState::INVISIBLE_FIXED_AS_UNOPT_BEGIN:
            state = BlockState::FIXED_AS_UNOPT_BEGIN;
            break;
        case BlockState::INVISIBLE_FIXED_AS_UNOPT_MIDDLE:
            state = BlockState::FIXED_AS_UNOPT_MIDDLE;
            break;
        case BlockState::INVISIBLE_FIXED_AS_UNOPT_END:
            state = BlockState::FIXED_AS_UNOPT_END;
            break;
        case BlockState::INVISIBLE_UNOPT_BEGIN:
            state = BlockState::UNOPT_BEGIN;
            break;
        case BlockState::INVISIBLE_UNOPT_MIDDLE:
            state = BlockState::UNOPT_MIDDLE;
            break;
        case BlockState::INVISIBLE_UNOPT_END:
            state = BlockState::UNOPT_END;
            break;
        case BlockState::INVISIBLE_OPTIMIZED:
            state = BlockState::OPTIMIZED;
            break;
        default:
            break;
    }
}

// Update the block's state in reading drive info phase 2
void Block::updateStateInDriveInfoPhase2() {
    switch (state) {
        case BlockState::FIXED_AS_UNOPT_BEGIN:
        case BlockState::FIXED_AS_UNOPT_MIDDLE:
        case BlockState::FIXED_AS_UNOPT_END:
            state = BlockState::FIXED;
            break;
        default:
            break;
    }
}

// Start moving
void Block::startMoving(int newX, int newY) {
  targetX = newX;
  targetY = newY;
  isMoving = true;
  // Set the position at the start of animation to the current position
  animX = x;
  animY = y;
}

// Update moving animation in defragmenting
void Block::updateMoving() {
  if (isMoving) {
    // Gradually move towards the target position
    float dx = targetX - animX;
    float dy = targetY - animY;
    
    if (abs(dx) < Config::Animation::POSITION_THRESHOLD && abs(dy) < Config::Animation::POSITION_THRESHOLD) {
      // When close enough, align completely with the target position
      animX = targetX;
      animY = targetY;
      isMoving = false;
    } else {
      // Gradually approach the target position
      animX += dx * Config::Animation::MOVE_SPEED;
      animY += dy * Config::Animation::MOVE_SPEED;
    }
  }
}

// Start explosion (from touch coordinates)
void Block::startExploding(int touchX, int touchY) {
  // Invisible or free areas do not explode
  switch (state) {
  case   BlockState::FREE:
  case   BlockState::INVISIBLE_FREE:
  case   BlockState::INVISIBLE_UNOPT_BEGIN:
  case   BlockState::INVISIBLE_UNOPT_MIDDLE:
  case   BlockState::INVISIBLE_UNOPT_END:
  case   BlockState::INVISIBLE_OPTIMIZED:
  case   BlockState::INVISIBLE_FIXED_AS_UNOPT_BEGIN:
  case   BlockState::INVISIBLE_FIXED_AS_UNOPT_MIDDLE:
  case   BlockState::INVISIBLE_FIXED_AS_UNOPT_END:
    return;
    break;
  
  default:
    break;
  }
  isExploding = true;
  
  // Calculate the screen coordinates of the block
  int screenX = Config::getGridOffsetX() + x * (Config::getBlockWidth() + 2) + Config::getBlockWidth() / 2;
  int screenY = Config::getGridOffsetY() + y * (Config::getBlockHeight() + 2) + Config::getBlockHeight() / 2;
  
  // Calculate the direction vector from the touch coordinates (screen coordinate system)
  float dirX = screenX - touchX;
  float dirY = screenY - touchY;
  
  // Calculate distance (add a small value to prevent division by zero)
  float distance = sqrt(dirX * dirX + dirY * dirY) + 0.1f;
  
  // Normalize the direction vector and set the velocity
  // Adjust so that the closer to the center, the greater the velocity
  float speed = 1.0f + (1.0f / distance) * 30.0f;
  explodeVelocityX = (dirX / distance) * speed;
  explodeVelocityY = (dirY / distance) * speed;
  
  // Change state to BAD
  state = BlockState::BAD;
}

// Update explosion animation
void Block::updateExplosion() {
  if (!isExploding) {
    return;
  }
  
  // Apply gravity in the Y-axis direction (increase velocity)
  explodeVelocityY += Config::Animation::GRAVITY;
  
  // Update position
  animX += explodeVelocityX;
  animY += explodeVelocityY;
}
