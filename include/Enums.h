/**
 * @file Enums.h
 * @brief Enumeration definitions for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains enumeration types used throughout the disk defragmentation
 * animation, including animation states and block states.
 */

#pragma once

// Animation states
enum class AnimationState {
  READING_DRIVE_INFO_PHASE1,  // Reading drive information (Phase 1)
  READING_DRIVE_INFO_PHASE2,  // Reading drive information (Phase 2: Identifying fixed data)
  DEFRAGMENTING,              // Defragmenting
  COMPLETED,                  // Completed
  TOUCHED                     // When screen is touched
};

// Block states
enum class BlockState {
  INVISIBLE_FREE,                   // Invisible free space
  INVISIBLE_UNOPT_BEGIN,            // Invisible unoptimized data (beginning of drive)
  INVISIBLE_UNOPT_MIDDLE,           // Invisible unoptimized data (middle of drive)
  INVISIBLE_UNOPT_END,              // Invisible unoptimized data (end of drive)
  INVISIBLE_OPTIMIZED,              // Invisible optimized data
  INVISIBLE_FIXED_AS_UNOPT_BEGIN,   // Invisible fixed data (beginning of drive)
  INVISIBLE_FIXED_AS_UNOPT_MIDDLE,  // Invisible fixed data (middle of drive)
  INVISIBLE_FIXED_AS_UNOPT_END,     // Invisible fixed data (end of drive)
  FIXED_AS_UNOPT_BEGIN,             // Fixed data (beginning of drive)
  FIXED_AS_UNOPT_MIDDLE,            // Fixed data (middle of drive)
  FIXED_AS_UNOPT_END,               // Fixed data (end of drive)
  FREE,                             // Free space
  UNOPT_BEGIN,                      // Unoptimized data (beginning of drive)
  UNOPT_MIDDLE,                     // Unoptimized data (middle of drive)
  UNOPT_END,                        // Unoptimized data (end of drive)
  OPTIMIZED,                        // Optimized data
  FIXED,                            // Fixed data
  READING,                          // Data that's currently being read
  WRITING,                          // Data that's currently being written
  BAD                               // Bad area
};
