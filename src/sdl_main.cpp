/**
 * @file sdl_main.cpp
 * @brief SDL main entry point for PC-based disk defragmentation simulator
 * @author cubic9com
 * @date 2025
 * @copyright Copyright (c) 2025 cubic9com All rights reserved.
 * 
 * This file provides the main entry point for running the disk defragmentation
 * simulator on PC using SDL. It utilizes the M5GFX library's SDL backend to
 * emulate the M5Stack display on desktop platforms.
 * 
 * Note: This file is only compiled when SDL_h_ macro is defined.
 * For actual M5Stack hardware, main.cpp is used instead.
 */

#include <M5GFX.h>
#if defined ( SDL_h_ )

void setup(void);
void loop(void);

__attribute__((weak))
int user_func(bool* running)
{
  setup();
  do
  {
    loop();
  } while (*running);
  return 0;
}

int main(int, char**)
{
  // The second argument is effective for step execution with breakpoints.
  // You can specify the time in milliseconds to perform slow execution that ensures screen updates.
  return lgfx::Panel_sdl::main(user_func, 128);
}

#endif
