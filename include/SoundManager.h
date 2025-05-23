/**
 * @file SoundManager.h
 * @brief Sound effects management for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file contains the SoundManager class which handles sound effects
 * for the disk defragmentation animation, including seek sounds and
 * explosion effects.
 */

#pragma once

#include <M5Unified.h>
#include <random>
#include "Config.h"

// Sound management class
class SoundManager {
private:
  std::mt19937& rng;
  
public:
  // Constructor
  SoundManager(std::mt19937& rng);

  // Initialization
  void initialize();

  // Play hard disk seek sound
  void playSeekSound();

  // Play explosion sound
  void playExplosionSound();
};
