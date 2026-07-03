#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "base.h"

#include "slime.c"
#include "game.c"
#include "render.c"

int main() {
  GameConfig config = {
      .screenWidth = 1080,
      .screenHeight = 720,
      .gameTitle = "game",
      .targetFps = 60,
      .exitKey = KEY_Q,
  };

  GameRun(&config);

  return 0;
}

/* vim:set ts=3 sw=2 sts=2 et: */
