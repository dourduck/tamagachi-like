void Render_Init(Render_Contex *render_contex) {
  int font_size = 32;
  Font font = LoadFontEx("./styles/Hack-Regular.ttf", font_size, NULL, 0);

  // GuiLoadStyle("./styles/cyber/style_cyber.rgs");
  // GuiLoadStyle("./styles/jungle/style_jungle.rgs");
  GuiLoadStyle("./styles/cherry/style_cherry.rgs");

  render_contex->font = font;
  GuiSetFont(font);

  GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
  // render_contex->sprite = LoadTexture("./art/blug_idle.png");
}

void Render_Close(Render_Contex *render_contex) {
  // GuiLoadStyleDefault();

  // UnloadFont(render_contex->font);
}

void Render_Update(Render_Contex *render_contex, World *world) {
  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  if (GuiButton((Rectangle){24, 24, 400, 100}, "#191#Show Message 0000")) {
    render_contex->showMessageBox = true;
  }

  if (GuiButton((Rectangle){424, 24, 400, 100}, "#191#Show Message 0000")) {
    // render_contex->showMessageBox = true;
  }

  if (render_contex->showMessageBox) {
    int result = GuiMessageBox((Rectangle){85 + 200, 70 + 100, 250, 100},
                               "#191#Message Box", "Hi! This is a message!",
                               "Nice;Cool");

    if (result >= 0) {
      render_contex->showMessageBox = false;
    }
  }

  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (world->traits[i] & TRAIT_ACTIVE) {
      Entity_RenderData render_data = world->render_data[i];

      TextureID tex_id = render_data.texture_id;
      Texture tex = world->texture_registry.textures[tex_id];
      Rectangle _src = render_data.src;
      Rectangle _dest = render_data.dest;
      Vector2 origin = render_data.origin;

      DrawTexturePro(tex, _src, _dest, origin, 0, WHITE);
    }
  }

  render_slime_targets(world);

  EndDrawing();
}

// #include "render.h"
// #include "game.h"
// #include "raylib.h"
// #include <stdio.h>

// #define RAYGUI_IMPLEMENTAION
// #include "raygui.h"

/* vvv DEBUG CONSLE vvv */

// void Draw_DebugConsole(Input *input) {
//   Rectangle r = {.x = 0, .y = 0, .width = GetRenderWidth(), .height = 300};
//   DrawRectangleRec(r, Fade(BLACK, 0.6));
//
//   int x = (input->mouseWorldPosition.x);
//   int y = (input->mouseWorldPosition.y);
//
//   DrawText(TextFormat("Mouse World Position: (%d, %d)", x, y), 10, 10, 24,
//            RAYWHITE);
// }
//
// /* ^^^ DEBUG CONSLE ^^^ */
//
// void RenderUpdate(GameCtx *gameContext, Input *input, Camera2D camera) {
//   BeginDrawing();
//   ClearBackground(DARKGRAY);
//
//   BeginMode2D(camera);
//
//   int fontSize = 80;
//   int text_x = -camera.offset.x + 20;
//   int text_y = -camera.offset.y + 20;
//
//   char buffer_text[32];
//   snprintf(buffer_text, sizeof(buffer_text), "WATER: %d",
//   gameContext->water); DrawText(buffer_text, text_x, text_y, fontSize,
//   SKYBLUE); text_y += (int)(fontSize * 1.5);
//
//   snprintf(buffer_text, sizeof(buffer_text), "RADS: %d", gameContext->rads);
//   DrawText(buffer_text, text_x, text_y, fontSize, LIME);
//   text_y += (int)(fontSize * 1.5);
//
//   snprintf(buffer_text, sizeof(buffer_text), "BOREDOM: %d",
//            gameContext->boredom);
//   DrawText(buffer_text, text_x, text_y, fontSize, MAROON);
//
//   DrawTexturePro(gameContext->currentSlimeTexture, gameContext->source,
//                  gameContext->destination, gameContext->origin,
//                  gameContext->rotation, gameContext->tint);
//
//   EndMode2D();
//
//
//   if (gameContext->console) {
//     Draw_DebugConsole(input);
//   }
//
//   EndDrawing();
// }

/* vim:set ts=3 sw=2 sts=2 et: */
