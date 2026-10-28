/**
 * @file UIRenderer.cpp
 * @brief Implementation of user interface rendering for disk defragmentation visualization
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the UIRenderer class which handles all user interface
 * rendering including the window frame, progress bar, grid display, and
 * hit counter for the disk defragmentation animation.
 */

#include "UIRenderer.h"
#include <M5Unified.h>

// Constructor
UIRenderer::UIRenderer()
  : state(AnimationState::READING_DRIVE_INFO_PHASE1),
    completionPercentage(0),
    screenWidth(0),
    screenHeight(0),
    hitCounter(0),
    hasPushedFrame(false),
    lastPushedState(AnimationState::READING_DRIVE_INFO_PHASE1),
    lastPushedPercentage(-1) {
}

// Initialization
void UIRenderer::initialize() {
  // Get and save the screen width once
  screenWidth = M5.Display.width();

  // Get and save the screen height once
  screenHeight = M5.Display.height();

  // Initialize hit counter
  hitCounter = 0;
}

// Draw UI elements
void UIRenderer::drawUI() {
  // Title bar
  canvas.fillRect(0, 0, screenWidth, Config::TITLE_BAR_HEIGHT, Colors::UI::TITLE_BAR_BACK);
  
  // Buttons in the title bar
  // Close button
  canvas.fillRect(screenWidth - 18, 3, 15, 14, Colors::UI::BUTTON_BACK);
  canvas.drawLine(screenWidth - 18, 3, screenWidth - 4, 3, Colors::UI::BUTTON_LIGHT);      // Drawing top edge (white)
  canvas.drawLine(screenWidth - 18, 3, screenWidth - 18, 16, Colors::UI::BUTTON_LIGHT);    // Drawing left edge (white)
  canvas.drawLine(screenWidth - 4, 3, screenWidth - 4, 16, Colors::UI::BUTTON_SHADOW);     // Drawing right edge (black)
  canvas.drawLine(screenWidth - 18, 16, screenWidth - 4, 16, Colors::UI::BUTTON_SHADOW);   // Drawing bottom edge (black)
  canvas.drawLine(screenWidth - 15, 7, screenWidth - 7, 11, Colors::UI::BUTTON_TEXT);      // Drawing ×
  canvas.drawLine(screenWidth - 15, 11, screenWidth - 7, 7, Colors::UI::BUTTON_TEXT);      // Drawing ×
  
  // Maximize button
  canvas.fillRect(screenWidth - 36, 3, 15, 14, Colors::UI::BUTTON_BACK);
  canvas.drawLine(screenWidth - 36, 3, screenWidth - 22, 3, Colors::UI::BUTTON_LIGHT);     // Drawing top edge (white)
  canvas.drawLine(screenWidth - 36, 3, screenWidth - 36, 16, Colors::UI::BUTTON_LIGHT);    // Drawing left edge (white)
  canvas.drawLine(screenWidth - 22, 3, screenWidth - 22, 16, Colors::UI::BUTTON_SHADOW);   // Drawing right edge (black)
  canvas.drawLine(screenWidth - 36, 16, screenWidth - 22, 16, Colors::UI::BUTTON_SHADOW);  // Drawing bottom edge (black)
  canvas.drawRect(screenWidth - 32, 7, 7, 6, Colors::UI::BUTTON_TEXT);                     // Drawing □
  
  // Minimize button
  canvas.fillRect(screenWidth - 54, 3, 15, 14, Colors::UI::BUTTON_BACK);
  canvas.drawLine(screenWidth - 54, 3, screenWidth - 40, 3, Colors::UI::BUTTON_LIGHT);     // Drawing top edge (white)
  canvas.drawLine(screenWidth - 54, 3, screenWidth - 54, 16, Colors::UI::BUTTON_LIGHT);    // Drawing left edge (white)
  canvas.drawLine(screenWidth - 40, 3, screenWidth - 40, 16, Colors::UI::BUTTON_SHADOW);   // Drawing right edge (black)
  canvas.drawLine(screenWidth - 54, 16, screenWidth - 40, 16, Colors::UI::BUTTON_SHADOW);  // Drawing bottom edge (black)
  canvas.drawLine(screenWidth - 50, 12, screenWidth - 44, 12, Colors::UI::BUTTON_TEXT);    // Drawing _
  
  // Title text
  canvas.setTextColor(Colors::UI::TITLE_BAR_TEXT);
  canvas.setTextSize(1);
  canvas.setCursor(5, 6);
  canvas.print("Defragmenting Drive C");
  
  // Status area at the bottom
  // Calculate from bottom of screen instead of grid bottom
  int statusY = screenHeight - 55;  // 55 pixels from bottom for status text
  int progressBarY = screenHeight - 40;  // 40 pixels from bottom for progress bar
  int percentageY = screenHeight - 18;   // 18 pixels from bottom for percentage
  
  // Status message
  canvas.setTextColor(Colors::UI::BUTTON_TEXT);
  canvas.setCursor(5, statusY);
  
  switch (state) {
    case AnimationState::READING_DRIVE_INFO_PHASE1:
    case AnimationState::READING_DRIVE_INFO_PHASE2:
      canvas.print("Reading drive information...");
      break;
    case AnimationState::DEFRAGMENTING:
      canvas.print("Defragmenting file system...");
      break;
    case AnimationState::COMPLETED:
      canvas.print("Defragmentation completed.");
      break;
    case AnimationState::TOUCHED:
      canvas.print("The drive was damaged!!!");
      break;
  }
  
  // Progress bar
  canvas.drawRect(Config::getProgressBarOffsetX(), progressBarY, Config::getProgressBarWidth(), Config::getProgressBarHeight(), Colors::UI::PROGRESS_FRAME);
  
  // Calculate number of blocks based on percentage
  // First calculate the maximum number of blocks that can fit
  int progressBarInnerWidth = Config::getProgressBarWidth() - 4;  // -4 for padding (2 on each side)
  int maxBlocks = (progressBarInnerWidth + Config::getProgressBarBlockSpacing() - Config::getProgressBarBlockWidth()) / Config::getProgressBarBlockSpacing();
  
  // Calculate how many blocks to show based on percentage
  int blocksToShow = (completionPercentage * maxBlocks + 99) / 100;  // +99 for rounding up
  
  // Fill the progress bar (blue blocks)
  for (int i = 0; i < blocksToShow; i++) {
    int blockX = Config::getProgressBarOffsetX() + 2 + i * Config::getProgressBarBlockSpacing();  // +2 for padding
    canvas.fillRect(blockX, progressBarY + 2, 
                   Config::getProgressBarBlockWidth(), Config::getProgressBarBlockHeight(), Colors::UI::PROGRESS_BLOCK);
  }
  
  // Completion percentage
  canvas.setCursor(5, percentageY);
  canvas.print(completionPercentage);
  canvas.print("% Complete");
}

// Draw hit counter
void UIRenderer::drawHitCounter() {
  // Hit counter
  canvas.setTextColor(Colors::UI::HIT_COUNTER);
  canvas.setTextSize(2);
  canvas.setCursor(screenWidth / 2 - 35, screenHeight / 2 - 10);
  canvas.print(hitCounter);
  if (hitCounter == 1) {
    canvas.print(" Hit!");
  } else {
    canvas.print(" Hits!");
  }

  if (hitCounter >= 10) {
    canvas.setCursor(screenWidth / 2 - 35, screenHeight / 2 + 20);
    canvas.print("GREAT!");
  }
}

// Draw grid and UI elements
void UIRenderer::draw(GridManager& gridManager, AnimationManager& animationManager) {
  // Clear the canvas
  canvas.fillScreen(Colors::UI::WINDOW_BACK);
  
  // Fill the grid area background with white
  canvas.fillRect(Config::getGridOffsetX() - 3, Config::getGridOffsetY() - 3, 
                 Config::getGridCols() * (Config::getBlockWidth() + 2) + 4, 
                 Config::getGridRows() * (Config::getBlockHeight() + 2) + 4, Colors::UI::GRID_BACK);
  
  // Grid area frame
  canvas.drawRect(Config::getGridOffsetX() - 3, Config::getGridOffsetY() - 3, 
                 Config::getGridCols() * (Config::getBlockWidth() + 2) + 4, 
                 Config::getGridRows() * (Config::getBlockHeight() + 2) + 4, Colors::UI::GRID_FRAME);
  
  // Draw UI elements
  drawUI();

  // Draw blocks
  for (int y = 0; y < gridManager.getRowCount(); y++) {
    for (int x = 0; x < gridManager.getColumnCount(); x++) {
      gridManager.getBlock(x, y).draw(state);
    }
  }

  // Draw hit counter
  if (state == AnimationState::TOUCHED) {
    drawHitCounter();
  }

  // Transfer only the changed parts of the canvas to the display
  pushChangedBands(gridManager);
}

// Compute a compact signature of how a cell is rendered: the block state
// plus the moving/exploding flags (a moving or exploding block is not
// drawn in its own cell)
int UIRenderer::cellSignature(const Block& block) const {
  return static_cast<int>(block.state) * 4 + (block.isMoving ? 2 : 0) + (block.isExploding ? 1 : 0);
}

// Transfer only the changed horizontal bands from the canvas to the display
void UIRenderer::pushChangedBands(GridManager& gridManager) {
  const int rows = gridManager.getRowCount();
  const int cols = gridManager.getColumnCount();
  const int cellCount = rows * cols;

  // First frame, or a grid of a different size: transfer everything and
  // start tracking from this frame
  if (!hasPushedFrame || static_cast<int>(cellSignatures.size()) != cellCount) {
    canvas.pushSprite(0, 0);

    cellSignatures.assign(cellCount, 0);
    lastAnimX.assign(cellCount, 0.0f);
    lastAnimY.assign(cellCount, 0.0f);
    lastDynamic.assign(cellCount, 0);
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        const Block& block = gridManager.getBlock(x, y);
        const int index = y * cols + x;
        cellSignatures[index] = cellSignature(block);
        lastAnimX[index] = block.animX;
        lastAnimY[index] = block.animY;
        lastDynamic[index] = (block.isMoving || block.isExploding) ? 1 : 0;
      }
    }
    hasPushedFrame = true;
    lastPushedState = state;
    lastPushedPercentage = completionPercentage;
    return;
  }

  // Find the screen rows whose content changed
  std::vector<char> dirtyRows(screenHeight, 0);
  auto markRows = [&dirtyRows, this](int top, int bottom) {
    if (top < 0) top = 0;
    if (bottom >= screenHeight) bottom = screenHeight - 1;
    for (int y = top; y <= bottom; y++) {
      dirtyRows[y] = 1;
    }
  };

  const int gridOffsetY = Config::getGridOffsetY();
  const int blockHeight = Config::getBlockHeight();
  const int blockStepY = blockHeight + 2;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      const Block& block = gridManager.getBlock(x, y);
      const int index = y * cols + x;

      // A cell whose appearance changed
      const int signature = cellSignature(block);
      if (signature != cellSignatures[index]) {
        cellSignatures[index] = signature;
        const int top = gridOffsetY + y * blockStepY;
        markRows(top - 1, top + blockHeight + 1);
      }

      // A moving or exploding block also changes the rows around its
      // previous and current animation positions
      if (lastDynamic[index]) {
        const int top = gridOffsetY + static_cast<int>(lastAnimY[index] * blockStepY);
        markRows(top - 1, top + blockHeight + 1);
      }
      const bool dynamic = block.isMoving || block.isExploding;
      if (dynamic) {
        const int top = gridOffsetY + static_cast<int>(block.animY * blockStepY);
        markRows(top - 1, top + blockHeight + 1);
        lastAnimX[index] = block.animX;
        lastAnimY[index] = block.animY;
      }
      lastDynamic[index] = dynamic ? 1 : 0;
    }
  }

  // The status area at the bottom changes with the state and the percentage
  if (state != lastPushedState || completionPercentage != lastPushedPercentage) {
    markRows(screenHeight - 64, screenHeight - 1);
  }
  lastPushedState = state;
  lastPushedPercentage = completionPercentage;

  // The hit counter is drawn over the center of the grid
  if (state == AnimationState::TOUCHED) {
    markRows(screenHeight / 2 - 12, screenHeight / 2 + 40);
  }

  // Count the changed rows
  int dirtyHeight = 0;
  for (int y = 0; y < screenHeight; y++) {
    if (dirtyRows[y]) dirtyHeight++;
  }
  if (dirtyHeight == 0) {
    return; // Nothing changed: skip the transfer entirely
  }
  if (dirtyHeight > screenHeight * 3 / 4) {
    canvas.pushSprite(0, 0); // Most of the screen changed: a full transfer is cheaper
    return;
  }

  // Transfer each contiguous band of changed rows from the canvas buffer
  const uint16_t* buffer = static_cast<const uint16_t*>(canvas.getBuffer());
  int y = 0;
  while (y < screenHeight) {
    if (!dirtyRows[y]) {
      y++;
      continue;
    }
    const int bandTop = y;
    while (y < screenHeight && dirtyRows[y]) {
      y++;
    }
    M5.Display.pushImage(0, bandTop, screenWidth, y - bandTop, buffer + bandTop * screenWidth);
  }
}

// Set state
void UIRenderer::setState(AnimationState newState) {
  state = newState;
}

// Get state
AnimationState UIRenderer::getState() const {
  return state;
}

// Set completion percentage
void UIRenderer::setCompletionPercentage(int percentage) {
  completionPercentage = percentage;
}

// Get completion percentage
int UIRenderer::getCompletionPercentage() const {
  return completionPercentage;
}

// Increment hit counter
void UIRenderer::incrementHitCounter() {
  hitCounter++;
}

// Get hit counter
int UIRenderer::getHitCounter() const {
  return hitCounter;
}
