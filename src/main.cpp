#include <M5Unified.h>
#include <M5GFX.h>
#include "Config.h"
#include "Colors.h"
#include "Enums.h"
#include "Block.h"
#include "GridManager.h"
#include "FileManager.h"
#include "AnimationManager.h"
#include "UIRenderer.h"
#include "SoundManager.h"
#include "DefragSimulator.h"

// Canvas for off-screen rendering
M5Canvas canvas(&M5.Display);

// Global instance
DefragSimulator defragSim;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  // Set screen orientation (landscape)
  M5.Display.setRotation(1);
  M5.Display.startWrite();
  
  // Initialize canvas (same size as screen)
  canvas.createSprite(M5.Display.width(), M5.Display.height());

  // Initialize defrag simulator
  defragSim.initialize();
}

void loop() {
  M5.update();
  
  // Check touch events
  if (M5.Touch.getCount() > 0) {
    auto touch = M5.Touch.getDetail();
    if (touch.wasPressed()) {
      // Process when touch is detected
      defragSim.handleTouch(touch.x, touch.y);
    }
  }

  // Update process
  defragSim.update();
  
  // Draw to canvas
  defragSim.draw();
  
  // Add a slight delay (adjust animation speed)
  delay(1);
}
