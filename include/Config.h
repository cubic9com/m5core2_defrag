/**
 * @file Config.h
 * @brief Configuration settings for disk defragmentation simulator
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the Config singleton class that manages all configuration
 * parameters for the disk defragmentation visualization, including grid dimensions,
 * UI settings, animation parameters, and sound configurations.
 */

#pragma once

#include <cstdint>

// Configuration singleton class for the defragmentation simulator
class Config {
private:
    static Config* instance;
    
    // Screen dimensions
    int screenWidth;
    int screenHeight;
    
    // Calculated values
    int gridCols;
    int gridRows;
    int progressBarWidth;
    int gridOffsetX;
    int gridOffsetY;
    int progressBarOffsetX;
    
    Config() = default;
    
public:
    static Config* getInstance();
    void initialize(int width, int height);
    
    // ========================================
    // Grid configuration
    // ========================================
    
    // Number of blocks in the horizontal direction of the grid
    static int getGridCols();
    
    // Number of blocks in the vertical direction of the grid
    static int getGridRows();
    
    // X-coordinate offset of the grid (distance from the left edge)
    static int getGridOffsetX();
    
    // Y-coordinate offset of the grid (distance from the top edge)
    static int getGridOffsetY();
    
    // ========================================
    // Block configuration
    // ========================================
    
    // Block width (in pixels)
    static int getBlockWidth();
    
    // Block height (in pixels)
    static int getBlockHeight();
    
    // ========================================
    // UI configuration
    // ========================================
    
    // Title bar height (in pixels)
    static constexpr int TITLE_BAR_HEIGHT = 20;
    
    // ========================================
    // Progress bar configuration
    // ========================================
    
    // Total width of the progress bar (in pixels)
    static int getProgressBarWidth();
    
    // X-coordinate offset of the progress bar
    static int getProgressBarOffsetX();
    
    // Height of the progress bar (in pixels)
    static int getProgressBarHeight();
    
    // Width of the progress block (in pixels)
    static int getProgressBarBlockWidth();
    
    // Height of the progress block (in pixels)
    static int getProgressBarBlockHeight();
    
    // Spacing between progress blocks (in pixels)
    static int getProgressBarBlockSpacing();
    
    // ========================================
    // Animation configuration
    // ========================================
    struct Animation {
        // Block movement speed coefficient
        static constexpr float MOVE_SPEED = 0.7f;
        
        // Position threshold (differences below this are ignored)
        static constexpr float POSITION_THRESHOLD = 0.05f;
        
        // Gravity acceleration during explosion
        static constexpr float GRAVITY = 0.4f;
        
        // Defrag step interval (in frames)
        static constexpr int DEFRAG_STEP_INTERVAL = 10;
        
        // Block movement delay time (in milliseconds)
        static constexpr int BLOCK_MOVE_DELAY = 5;
        
        // Wait time until reset (in milliseconds)
        static constexpr int RESET_DELAY = 3000;
    };
    
    // ========================================
    // Sound configuration
    // ========================================
    struct Sound {
        // Seek sound configuration
        struct Seek {
            // Base frequency (Hz)
            static constexpr int BASE_FREQ = 4500;
            
            // Random frequency variation range (Hz)
            static constexpr int FREQ_VARIATION = 500;
            
            // Minimum sound duration (milliseconds)
            static constexpr int MIN_DURATION = 4;
            
            // Maximum sound duration (milliseconds)
            static constexpr int MAX_DURATION = 10;
            
            // Number of consecutive beep sounds
            static constexpr int BEEP_COUNT = 6;
            
            // Speaker volume
            static constexpr int SPEAKER_VOLUME = 10;
        };
        
        // Explosion sound configuration
        struct Explosion {
            // Explosion sound frequency
            static constexpr float FREQUENCY = 82.407F;
            
            // Explosion sound duration
            static constexpr uint32_t DURATION = 50U;
            
            // Speaker volume
            static constexpr int SPEAKER_VOLUME = 150;
        };
    };
};
