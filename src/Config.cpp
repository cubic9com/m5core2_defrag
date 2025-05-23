/**
 * @file Config.cpp
 * @brief Implementation of configuration settings for disk defragmentation simulator
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the Config singleton class that manages all configuration
 * parameters for the disk defragmentation visualization, including grid dimensions,
 * UI settings, animation parameters, and sound configurations.
 */

#include "Config.h"

// Initialize static member
Config* Config::instance = nullptr;

// Get the singleton instance of Config
Config* Config::getInstance() {
    if (instance == nullptr) {
        instance = new Config();
    }
    return instance;
}

// Initialize configuration with screen dimensions
void Config::initialize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    
    // ========================================
    // Block dimensions (fixed values)
    // ========================================
    const int blockWidth = 6;      // Width of each block in pixels
    const int blockHeight = 8;     // Height of each block in pixels
    const int blockSpacing = 2;    // Spacing between blocks in pixels
    
    // ========================================
    // Calculate grid columns and X offset
    // ========================================
    
    // Reserve minimum margins on both sides
    int maxGridAreaWidth = screenWidth - 10;  // 5 pixels margin on each side
    gridCols = maxGridAreaWidth / (blockWidth + blockSpacing);
    
    
    // Calculate actual grid pixel width
    int actualGridWidth = gridCols * (blockWidth + blockSpacing);
    
    // Center the grid horizontally
    gridOffsetX = (screenWidth - actualGridWidth) / 2;
    
    // ========================================
    // Calculate grid rows and Y offset
    // ========================================
    
    // Y offset includes title bar height and matches X offset for symmetry
    gridOffsetY = TITLE_BAR_HEIGHT + gridOffsetX;
    
    // Calculate available height for grid
    int statusAreaHeight = 65;  // Space for status text, progress bar, and percentage
    int gridAreaHeight = screenHeight - gridOffsetY - statusAreaHeight;
    gridRows = gridAreaHeight / (blockHeight + blockSpacing);
    
    // Ensure minimum grid rows for playability
    if (gridRows < 8) gridRows = 8;
    
    // ========================================
    // Calculate progress bar dimensions
    // ========================================
    
    // Progress bar block dimensions
    const int progressBlockWidth = 8;     // Width of each progress block
    const int progressBlockSpacing = 10;  // Spacing between progress blocks
    const int progressPadding = 2;        // Internal padding of progress bar frame
    
    // Calculate maximum number of progress blocks that can fit
    int maxProgressBarInnerWidth = screenWidth - 20 - 2 * progressPadding;  // 20px for margins, 4px for padding
    int maxBlocks = (maxProgressBarInnerWidth + progressBlockSpacing - progressBlockWidth) / progressBlockSpacing;
    
    // Calculate exact progress bar width based on number of blocks
    // Formula: first block width + (number of blocks - 1) * spacing
    int progressBarInnerWidth = progressBlockWidth + (maxBlocks - 1) * progressBlockSpacing;
    progressBarWidth = progressBarInnerWidth + 2 * progressPadding;  // Add frame padding
    
    // Center the progress bar horizontally
    progressBarOffsetX = (screenWidth - progressBarWidth) / 2;
}

// ========================================
// Grid configuration getters
// ========================================

// Get the number of grid columns
int Config::getGridCols() { 
    return getInstance()->gridCols; 
}

// Get the number of grid rows
int Config::getGridRows() { 
    return getInstance()->gridRows; 
}

// Get the X offset of the grid
int Config::getGridOffsetX() { 
    return getInstance()->gridOffsetX; 
}

// @brief Get the Y offset of the grid
int Config::getGridOffsetY() { 
    return getInstance()->gridOffsetY; 
}

// ========================================
// Block configuration getters
// ========================================

// Get the width of a single block
int Config::getBlockWidth() { 
    return 6;  // Fixed block width
}

// Get the height of a single block
int Config::getBlockHeight() {
    return 8;  // Fixed block height
}

// ========================================
// Progress bar configuration getters
// ========================================

// Get the total width of the progress bar
int Config::getProgressBarWidth() { 
    return getInstance()->progressBarWidth; 
}

// Get the X offset of the progress bar
int Config::getProgressBarOffsetX() { 
    return getInstance()->progressBarOffsetX; 
}

// Get the height of the progress bar
int Config::getProgressBarHeight() { 
    return 15;  // Fixed progress bar height
}

// Get the width of a single progress block
int Config::getProgressBarBlockWidth() {
    return 8;  // Fixed progress block width
}

// Get the height of a single progress block
int Config::getProgressBarBlockHeight() {
    return 11;  // Fixed progress block height
}

// Get the spacing between progress blocks
int Config::getProgressBarBlockSpacing() {
    return 10;  // Fixed spacing between progress blocks
}
