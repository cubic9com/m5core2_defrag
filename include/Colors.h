#pragma once

#include <cstdint>

// Organize color definitions
namespace Colors {
  // Color definitions for blocks
  namespace Block {
    constexpr uint16_t FREE = 0xFFFF;            // Color for free space
    constexpr uint16_t FIXED_BACK = 0xFFFF;      // Background color for fixed data
    constexpr uint16_t FIXED_FORE = 0xF800;      // Foreground color for fixed data
    constexpr uint16_t OPTIMIZED = 0x161F;       // Color for optimized data
    constexpr uint16_t UNOPT_BEGIN = 0x07FF;     // Color for unoptimized data (beginning of drive)
    constexpr uint16_t UNOPT_MIDDLE = 0x0430;    // Color for unoptimized data (middle of drive)
    constexpr uint16_t UNOPT_END = 0x0210;       // Color for unoptimized data (end of drive)
    constexpr uint16_t READING = 0x07E0;         // Color for data that's currently being read
    constexpr uint16_t WRITING = 0xF800;         // Color for data that's currently being written
    constexpr uint16_t BAD_BACK = 0xFFFF;        // Background color for bad area
    constexpr uint16_t BAD_FORE = 0xF800;        // Foreground color for bad area
    constexpr uint16_t FRAME = 0x0000;           // Color for block frame
  }

  // Color definitions for UI
  namespace UI {
    constexpr uint16_t WINDOW_BACK = 0xC618;     // Color for window background
    constexpr uint16_t TITLE_BAR_BACK = 0x0010;  // Color for window title bar background
    constexpr uint16_t TITLE_BAR_TEXT = 0xFFFF;  // Color for window title bar text
    constexpr uint16_t BUTTON_BACK = 0xC618;     // Color for button background
    constexpr uint16_t BUTTON_LIGHT = 0xFFFF;    // Color for button highlight
    constexpr uint16_t BUTTON_SHADOW = 0x0000;   // Color for button shadow
    constexpr uint16_t BUTTON_TEXT = 0x0000;     // Color for button text
    constexpr uint16_t GRID_BACK = 0xFFFF;       // Color for grid background
    constexpr uint16_t GRID_FRAME = 0x0000;      // Color for grid frame
    constexpr uint16_t PROGRESS_FRAME = 0x0000;  // Color for progress bar frame
    constexpr uint16_t PROGRESS_BLOCK = 0x0210;  // Color for progress bar block
    constexpr uint16_t HIT_COUNTER = 0xF800;     // Color for hit counter text
  }
}
