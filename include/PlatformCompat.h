/**
 * @file PlatformCompat.h
 * @brief Platform compatibility layer for cross-platform support
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file provides platform-specific compatibility definitions to enable
 * the code to run on both M5Stack and native PC/SDL environments.
 */

#pragma once

// Platform-specific compatibility layer for time and random functions

#ifdef ARDUINO
// M5Stack environment

#include <Arduino.h>
#include <esp_random.h>

// Arduino already provides millis() and delay() functions
// No need to redefine them

#define seed() (millis() + esp_random())

#else
// Native environment (PC/SDL build)

#include <SDL2/SDL.h>
#include <random>

#define millis() SDL_GetTicks()
#define delay(msec) SDL_Delay(msec)
#define seed() (SDL_GetTicks() + std::random_device{}())

#endif
