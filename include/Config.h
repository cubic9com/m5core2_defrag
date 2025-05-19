#pragma once

#include <cstdint>

namespace Config {
  namespace Grid {
    constexpr int WIDTH = 39;                    // Number of blocks in the horizontal direction of the grid
    constexpr int HEIGHT = 16;                   // Number of blocks in the vertical direction of the grid
    constexpr int OFFSET_X = 5;                  // X-coordinate offset of the grid (distance from the left edge)
    constexpr int OFFSET_Y = 25;                 // Y-coordinate offset of the grid (distance from the top edge)
  }

  namespace Block {
    constexpr int WIDTH = 6;                     // Block width (in pixels)
    constexpr int HEIGHT = 8;                    // Block height (in pixels)
  }

  namespace UI {
    constexpr int TITLE_BAR_HEIGHT = 20;         // Title bar height (in pixels)
    
    namespace ProgressBar {
      constexpr int WIDTH = 302;                 // Total width of the progress bar (in pixels)
      constexpr int HEIGHT = 15;                 // Height of the progress bar (in pixels)
      constexpr int BLOCK_WIDTH = 8;             // Width of the progress block (in pixels)
      constexpr int BLOCK_HEIGHT = 11;           // Height of the progress block (in pixels)
      constexpr int BLOCK_SPACING = 10;          // Spacing between progress blocks (in pixels)
    }
  }

  namespace Animation {
    constexpr float MOVE_SPEED = 0.7f;           // Block movement speed coefficient
    constexpr float POSITION_THRESHOLD = 0.05f;  // Position threshold (differences below this are ignored)
    constexpr float GRAVITY = 0.4f;              // Gravity acceleration during explosion
    constexpr int DEFRAG_STEP_INTERVAL = 10;     // Defrag step interval (in frames)
    constexpr int BLOCK_MOVE_DELAY = 5;          // Block movement delay time (in milliseconds)
    constexpr int RESET_DELAY = 3000;            // Wait time until reset (in milliseconds)
  }

  namespace Sound {
    namespace Seek {
      constexpr int BASE_FREQ = 4500;            // Base frequency (Hz)
      constexpr int FREQ_VARIATION = 500;        // Random frequency variation range (Hz)
      constexpr int MIN_DURATION = 4;            // Minimum sound duration (milliseconds)
      constexpr int MAX_DURATION = 10;           // Maximum sound duration (milliseconds)
      constexpr int BEEP_COUNT = 6;              // Number of consecutive beep sounds
      constexpr int SPEAKER_VOLUME = 10;         // Speaker volume
    }

    namespace Explosion {
      const float FREQUENCY = 82.407F;           // Explosion sound frequency
      const uint32_t DURATION = 50U;             // Explosion sound duration
      constexpr int SPEAKER_VOLUME = 150;        // Speaker volume
    }
  }
}
