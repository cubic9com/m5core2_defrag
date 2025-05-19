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
