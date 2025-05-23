/**
 * @file SoundManager.cpp
 * @brief Implementation of sound effects management for disk defragmentation simulation
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file implements the SoundManager class which handles sound effects
 * for the disk defragmentation animation, including seek sounds and
 * explosion effects.
 */

#include "Config.h"
#include "SoundManager.h"
#include "PlatformCompat.h"

// Constructor
SoundManager::SoundManager(std::mt19937& rng)
  : rng(rng) {
  initialize();
}

// Initialization
void SoundManager::initialize() {
  // Set speaker volume
  M5.Speaker.setVolume(Config::Sound::Seek::SPEAKER_VOLUME);
}

// Play hard disk seek sound
void SoundManager::playSeekSound() {
  // Play a series of short noises with randomized frequency and length
  std::uniform_int_distribution<int> freqDist(-Config::Sound::Seek::FREQ_VARIATION, Config::Sound::Seek::FREQ_VARIATION);
  std::uniform_int_distribution<int> durDist(Config::Sound::Seek::MIN_DURATION, Config::Sound::Seek::MAX_DURATION);
  std::uniform_int_distribution<int> delayDist(0, 2);
  
  // Play multiple short noises in succession
  for (int i = 0; i < Config::Sound::Seek::BEEP_COUNT; i++) {
    // Add significant randomness to the base frequency
    int freq = Config::Sound::Seek::BASE_FREQ + freqDist(rng);
    // Also randomize the duration
    int duration = durDist(rng);
    // Play the noise
    M5.Speaker.tone(freq, duration);
    // Leave a very short interval (this is also slightly randomized)
    delay(delayDist(rng));
  }
}

// Play explosion sound
void SoundManager::playExplosionSound() {
  // Set speaker volume
  M5.Speaker.setVolume(Config::Sound::Explosion::SPEAKER_VOLUME);
  // Play explosion sound
  M5.Speaker.tone(Config::Sound::Explosion::FREQUENCY, Config::Sound::Explosion::DURATION);
}
