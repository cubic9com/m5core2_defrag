/**
 * @file DefragSimulator.cpp
 * @brief Implementation of main controller for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the DefragSimulator class which orchestrates the entire
 * disk defragmentation animation, managing all subsystems including grid,
 * file management, animations, UI rendering, and sound effects.
 */

#include "DefragSimulator.h"
#include "PlatformCompat.h"

// Constructor
DefragSimulator::DefragSimulator() 
  : gridManager(),
    fileManager(gridManager),
    animationManager(gridManager),
    uiRenderer(),
    soundManager(gridManager.getRNG()),
    completedStateStartTime(0),
    isInCompletedState(false),
    touchStateStartTime(0),
    isInTouchState(false) {
  
  // Set initial state
  uiRenderer.setState(AnimationState::READING_DRIVE_INFO_PHASE1);
}

// Initialization
void DefragSimulator::initialize() {
  // Initialize UI rendering class
  uiRenderer.initialize();

  // Initialize sound class
  soundManager.initialize();

  // Initial animation state is reading drive information phase 1
  setState(AnimationState::READING_DRIVE_INFO_PHASE1);
  uiRenderer.setCompletionPercentage(0);
  
  // Reset completion state flag
  completedStateStartTime = 0;
  isInCompletedState = false;

  // Reset touch state flag
  touchStateStartTime = 0;
  isInTouchState = false;

  // Play seek sound
  soundManager.playSeekSound();
}

// Reset
void DefragSimulator::reset() {
  // Create a new grid manager
  gridManager = GridManager();
  
  // Reinitialize file manager (create a new instance)
  new (&fileManager) FileManager(gridManager);
  
  // Reset animation manager
  animationManager.reset();
  
  // Set initial state
  initialize();
}

// Update process
void DefragSimulator::update() {
  AnimationState state = uiRenderer.getState();
  int completionPercentage = 0;
  
  switch (state) {
    case AnimationState::READING_DRIVE_INFO_PHASE1:
      // Update blocks
      animationManager.updateBlocksInDriveInfoPhase1();
      
      // Update progress
      completionPercentage = animationManager.calculateDriveInfoPhasesCompletionPercentage();
      uiRenderer.setCompletionPercentage(completionPercentage);
      
      // Check if scan is complete
      if (animationManager.isDriveInfoPhase1ScanCompleted()) {
        // Change animation state to reading drive information (Phase 2: Identifying immovable data)
        uiRenderer.setState(AnimationState::READING_DRIVE_INFO_PHASE2);
      }
      break;
  
    case AnimationState::READING_DRIVE_INFO_PHASE2:
      // Update blocks
      animationManager.updateBlocksInDriveInfoPhase2();
      
      // Update progress
      completionPercentage = animationManager.calculateDriveInfoPhasesCompletionPercentage();
      uiRenderer.setCompletionPercentage(completionPercentage);
      
      // Check if scan is complete
      if (animationManager.isDriveInfoPhase2ScanCompleted()) {
        // Change animation state to DEFRAGMENTING
        uiRenderer.setState(AnimationState::DEFRAGMENTING);
      }
      break;
  
    case AnimationState::DEFRAGMENTING:
      // Update defrag step
      animationManager.incrementDefragStep();
      
      // If not moving a file, start moving a new file
      if (!fileManager.isMoving() && 
          animationManager.getDefragStep() % Config::Animation::DEFRAG_STEP_INTERVAL == 0) {
        fileManager.moveNextFile();
      }
      
      // Update blocks
      animationManager.updateBlocksInDefragmenting();
      
      // Update file movement
      fileManager.updateFileMovement();
      
      // Update progress
      completionPercentage = animationManager.calculateDefragmentingCompletionPercentage();
      uiRenderer.setCompletionPercentage(completionPercentage);
      
      // Play seek sound
      soundManager.playSeekSound();
      
      // Check if all fragmented files have been moved
      if (completionPercentage >= 100) {
        uiRenderer.setState(AnimationState::COMPLETED);
      }
      break;
      
    case AnimationState::COMPLETED:
      // If it's the first time entering COMPLETED state, record the time
      if (!isInCompletedState) {
        completedStateStartTime = millis();
        isInCompletedState = true;
      }
      
      // Reset after the set time has elapsed
      if (millis() - completedStateStartTime >= Config::Animation::RESET_DELAY) {
        reset();
      }
      break;

    case AnimationState::TOUCHED:
      // If it's the first time entering TOUCHED state, record the time
      if (!isInTouchState) {
        touchStateStartTime = millis();
        isInTouchState = true;
      }  
      
      // Update explosion animation
      for (int y = 0; y < gridManager.getRowCount(); y++) {
        for (int x = 0; x < gridManager.getColumnCount(); x++) {
          Block& block = gridManager.getBlock(x, y);
          block.updateExplosion();
        }
      }
      
      // Reset after the set time has elapsed
      if (millis() - touchStateStartTime >= Config::Animation::RESET_DELAY) {
        reset();
      }
      break;
  }
}

// Draw grid and UI elements
void DefragSimulator::draw() {
  uiRenderer.draw(gridManager, animationManager);
}

// Get state
AnimationState DefragSimulator::getState() const {
  return uiRenderer.getState();
}

// Set state
void DefragSimulator::setState(AnimationState newState) {
  uiRenderer.setState(newState);
}

// Process when touch is detected
void DefragSimulator::handleTouch(int x, int y) {
  // Set to TOUCHED state
  uiRenderer.setState(AnimationState::TOUCHED);
  
  // Increment hit counter
  uiRenderer.incrementHitCounter();
  
  // Explode using the touched screen coordinates as the reference point
  setAllBlocksToBAD(x, y);
}

// Explode all blocks
void DefragSimulator::setAllBlocksToBAD(int touchX, int touchY) {
  // Start explosion animation for all blocks
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      Block& block = gridManager.getBlock(x, y);
      // Explode blocks other than invisible or free areas
      switch (block.state) {
      case   BlockState::FREE:
      case   BlockState::INVISIBLE_UNOPT_BEGIN:
      case   BlockState::INVISIBLE_UNOPT_MIDDLE:
      case   BlockState::INVISIBLE_UNOPT_END:
      case   BlockState::INVISIBLE_OPTIMIZED:
      case   BlockState::INVISIBLE_FIXED_AS_UNOPT_BEGIN:
      case   BlockState::INVISIBLE_FIXED_AS_UNOPT_MIDDLE:
      case   BlockState::INVISIBLE_FIXED_AS_UNOPT_END:
        break;
      
      default:
            block.startExploding(touchX, touchY);
      }
    }
  }
  
  // Play explosion sound
  soundManager.playExplosionSound();
}
