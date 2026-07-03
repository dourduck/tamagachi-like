#include "raylib.h"
#include "slime.h"
World World_Create() {
  World world = {.next_entity_id = 0};

  // is this necessary?
  // memset(&world.traits, 0, sizeof(Entity_Traits) * MAX_ENTITIES);
  // memset(&world.position, 0, sizeof(Vector2) * MAX_ENTITIES);
  // memset(&world.velocity, 0, sizeof(Vector2) * MAX_ENTITIES);
  // memset(&world.render_data, 0, sizeof(Entity_RenderData) * MAX_ENTITIES);

  return world;
}

i32 Entity_NextID(World *world) {
  i32 entity_id = world->next_entity_id;
  world->next_entity_id++;
  return entity_id;
}

void Entity_Position_Set(World *world, i32 entity_id, float x, float y) {
  world->position[entity_id].x = x;
  world->position[entity_id].y = y;
}

void Entity_Velocity_Set(World *world, i32 entity_id, float dx, float dy) {
  world->velocity[entity_id].x = dx;
  world->velocity[entity_id].y = dy;
}

void Game_Update(World *world) {
  system_update_slimes(world, GetFrameTime());

  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (world->traits[i] & TRAIT_ACTIVE) {
      Entity_RenderData render_data = world->render_data[i];

      world->render_data[i].dest.x = world->position[i].x;
      world->render_data[i].dest.y = world->position[i].y;
      world->render_data[i].dest.width =
          render_data.src.width * render_data.scale;
      world->render_data[i].dest.height =
          render_data.src.height * render_data.scale;
    }
  }

}

void GameRun(GameConfig *config) {
  // SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(config->screenWidth, config->screenHeight, config->gameTitle);
  SetTargetFPS(config->targetFps);
  SetExitKey(config->exitKey);

  Render_Contex render_contex = {.showMessageBox = false,
                                 .font = GetFontDefault()};

  Render_Init(&render_contex);

  World world = World_Create();

  TextureRegistry_Load(&world);

  spawn_slimes(&world);

  // create slimes
  // for (int i = 0; i < 8; i++) {
  //   i32 entity_id = Entity_NextID(&world);
  //   world.traits[entity_id] |= TRAIT_ACTIVE;
  //
  //   int x = GetRandomValue(100, 900);
  //   int y = GetRandomValue(100, 600);
  //
  //   Entity_Position_Set(&world, entity_id, x, y);
  //   Entity_RenderData_Create(&world, entity_id, (TextureID)GetRandomValue(0, 2),
  //                            2);
  // }


  while (!WindowShouldClose()) {
    Game_Update(&world);
    Render_Update(&render_contex, &world);
  }

  Render_Close(&render_contex);

  TextureRegistry_Unload(&world);

  CloseWindow();
}

void TextureRegistry_Load(World *world) {
  char *paths[3] = {
      "./art/blug_idle.png",
      "./art/blug_happy.png",
      "./art/blug_sad.png",
  };

  TextureRegistry registry;

  for (int i = 0; i < MAX_TEXTURES; i++) {
    registry.textures[i] = LoadTexture(paths[i]);
  }

  world->texture_registry = registry;
}

void TextureRegistry_Unload(World *world) {
  for (int i = 0; i < MAX_TEXTURES; i++) {
    UnloadTexture(world->texture_registry.textures[i]);
  }
}

void Entity_RenderData_Create(World *world, i32 entity_id, TextureID texture_id,
                              float scale) {
  world->render_data[entity_id].scale = scale;
  world->render_data[entity_id].texture_id = texture_id;

  int tex_width = world->texture_registry.textures[texture_id].width;
  int tex_height = world->texture_registry.textures[texture_id].height;

  Rectangle _src = {.x = 0, .y = 0, .width = tex_width, .height = tex_height};

  world->render_data[entity_id].src = _src;

  int _x = world->position->x;
  int _y = world->position->y;
  int tex_width_scaled = tex_width * scale;
  int tex_height_scaled = tex_height * scale;

  Rectangle _dest = {.x = _x,
                     .y = _y,
                     .width = tex_height_scaled,
                     .height = tex_height_scaled};

  world->render_data[entity_id].dest = _dest;
  world->render_data[entity_id].origin = Vector2Zero();
}

// typedef struct {
//   Vector2 mousePosition;
//   Vector2 mouseWorldPosition;
//
//   bool keyPressed_F;
//   bool keyPressed_R;
//   bool keyPressed_GRAVE;
//   bool mouseLeftPressed;
//
// } Input;

// static void execute(Command command, void *context_ptr);

// static void execute(Command command, void *context_ptr);
//
//
//
//
//
// typedef enum {
//   TEXTURE_NONE = 0,
//   TEXTURE_SLIME_IDLE = 1,
//   TEXTURE_SLIME_HAPPY = 2,
//   TEXTURE_SLIME_SAD = 3,
// } TextureID;
//
// static void execute(Command command, void *context_ptr) {
//   GameCtx *gameCtx = (GameCtx *)context_ptr;
//
//   switch (command.kind) {
//   case CMD_WATER_UPDATE:
//     if (gameCtx->water > -(command.data.updateWater.amount)) {
//       gameCtx->water += command.data.updateWater.amount;
//     } else {
//       gameCtx->water = 0;
//     }
//     break;
//   case CMD_RADS_UPDATE:
//     if (gameCtx->rads > -(command.data.updateRads.amount)) {
//       gameCtx->rads += command.data.updateRads.amount;
//     } else {
//       gameCtx->rads = 0;
//     }
//     break;
//   case CMD_BOREDOM_UPDATE:
//     if (gameCtx->boredom < (100 - command.data.updateBoredom.amount)) {
//       gameCtx->boredom += command.data.updateBoredom.amount;
//     } else {
//       gameCtx->boredom = 100;
//     }
//     break;
//   case CMD_UPDATE_TEXTURE:
//     if (gameCtx->boredom >= 70 || gameCtx->water <= 30 || gameCtx->rads <=
//     30) {
//       gameCtx->currentSlimeTexture = gameCtx->slimeTexture_sad;
//     } else if (gameCtx->boredom <= 20 && gameCtx->water >= 80 &&
//                gameCtx->rads >= 80) {
//       gameCtx->currentSlimeTexture = gameCtx->slimeTexture_happy;
//     } else {
//       gameCtx->currentSlimeTexture = gameCtx->slimeTexture_idle;
//     }
//
//     // switch (command.data.updateTexture.textureID) {
//     // case TEXTURE_SLIME_IDLE:
//     //   gameCtx->currentSlimeTexture = gameCtx->slimeTexture_idle;
//     //   break;
//     // case TEXTURE_SLIME_HAPPY:
//     //   gameCtx->currentSlimeTexture = gameCtx->slimeTexture_happy;
//     //   break;
//     // case TEXTURE_SLIME_SAD:
//     //   gameCtx->currentSlimeTexture = gameCtx->slimeTexture_sad;
//     //   break;
//     // }
//     break;
//   default:
//     break;
//   }
// }
//
// void EntityVelocitySet(World *world, EntityID entityID, float dx, float dy) {
//   world->dx[entityID] = dx;
//   world->dy[entityID] = dy;
// }
//
// void EntityVelocityApply(World *world, EntityID entityID, float dt) {
//   world->x[entityID] += (world->dx[entityID] * dt);
//   world->y[entityID] += (world->dy[entityID] * dt);
// }
//
// World World_Create() { return (World){.nextID = 1}; }
//
// EntityID WorldEntity_Create(World *world) {
//   if (world->nextID <= ENTITY_MAX) {
//     EntityID entityID = world->nextID++;
//     world->active[entityID] = true;
//     return entityID;
//   } else {
//     return NIL;
//   }
// }
//
// void WorldEntity_TraitsAdd(World *world, EntityID entityID, TraitFlag flag) {
//   world->traits[entityID] |= flag;
// }
//
// void WorldEntity_TraitsRemove(World *world, EntityID entityID, TraitFlag
// flag) {
//   world->traits[entityID] &= ~(flag);
// }
//
// void WorldEntity_TraitsClear(World *world, EntityID entityID) {
//   world->traits[entityID] = TRAITS_NIL;
// }
//
// bool WorldEntity_HasTrait(World *world, EntityID entityID, TraitFlag flag) {
//   return (world->traits[entityID] & flag) == flag;
// }
//
// void WorldEntity_Remove(World *world, EntityID entityID) {
//   world->active[entityID] = false;
//   world->x[entityID] = NIL;
//   world->y[entityID] = NIL;
//   world->dx[entityID] = NIL;
//   world->dy[entityID] = NIL;
//   world->traits[entityID] = TRAITS_NIL;
// }
//
// void WorldEntity_SetPosition(World *world, EntityID entityID, float x,
//                              float y) {
//   world->x[entityID] = x;
//   world->y[entityID] = y;
// }
//
// void WorldEntity_SetVelocity(World *world, EntityID entityID, float dx,
//                              float dy) {
//   world->dx[entityID] = dx;
//   world->dy[entityID] = dy;
// }
//
// void InputPull(Input *input, Camera2D camera) {
//   Vector2 mouse_position = GetMousePosition();
//
//   input->mousePosition.x = mouse_position.x;
//   input->mousePosition.y = mouse_position.y;
//
//   Vector2 mouse_world_position = GetScreenToWorld2D(mouse_position, camera);
//   input->mouseWorldPosition.x = mouse_world_position.x;
//   input->mouseWorldPosition.y = mouse_world_position.y;
//
//   input->keyPressed_F = IsKeyPressed(KEY_F);
//   input->keyPressed_R = IsKeyPressed(KEY_R);
//   input->mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
//   input->keyPressed_GRAVE = IsKeyPressed(KEY_GRAVE);
//
//   // int x = (int)(floor(mouseWorldPosition.x / cellSize)) * cellSize;
//   // int y = (int)(floor(mouseWorldPosition.y / cellSize)) * cellSize;
//   // input->mouseWorldPositionQuantized.x = x;
//   // input->mouseWorldPositionQuantized.y = y;
// }
//
// // TODO:
// /*- Swap slime textures based on slime health status
//  *  Limit status update commands (rads, water, boredom) per tick (~3 or so?)
//  *  Play sound effects per status update command and when slime is sick
//  *  Description text for slime status (Depleted, thirsty, bored, happy, sad,
//  * etc)
//  * */
//
// void GameUpdate(Ring *ring, Input *input, float dt) {
//   if (input->keyPressed_F) {
//     ToggleFullscreen();
//   } else if (input->keyPressed_GRAVE) {
//     // gameContext->console = !gameContext->console;
//   } else if (IsKeyPressed(KEY_ONE)) {
//   } else if (IsKeyPressed(KEY_TWO)) {
//     Command command = {.kind = CMD_UPDATE_TEXTURE,
//                        .data.updateTexture = {TEXTURE_SLIME_HAPPY}};
//     RingCommandPush(ring, command);
//   } else if (IsKeyPressed(KEY_THREE)) {
//     Command command = {.kind = CMD_UPDATE_TEXTURE,
//                        .data.updateTexture = {TEXTURE_SLIME_SAD}};
//     RingCommandPush(ring, command);
//   }
// }
//
// void Tick(Ring *ring) {
//   Command updateRads = {
//       .kind = CMD_RADS_UPDATE,
//       .data = {-3},
//   };
//
//   RingCommandPush(ring, updateRads);
//
//   Command updateWater = {
//       .kind = CMD_WATER_UPDATE,
//       .data = {-10},
//   };
//
//   RingCommandPush(ring, updateWater);
//
//   Command updateBoredom = {
//       .kind = CMD_BOREDOM_UPDATE,
//       .data = {5},
//   };
//
//   RingCommandPush(ring, updateBoredom);
//
//   Command command = {.kind = CMD_UPDATE_TEXTURE,
//                      .data.updateTexture = {TEXTURE_SLIME_IDLE}};
//   RingCommandPush(ring, command);
// }
//
// void GameRun(GameConfig *config) {
//
//   Ring ring;
//   RingInitialize(&ring);
//
//   World world = World_Create();
//
//   GameCtx gameContext = {
//       .world = &world,
//       .water = 100,
//       .rads = 100,
//       .boredom = 0,
//   };
//
//   StrWrite(TEXT_BORED, &gameContext.bored);
//   StrWrite(TEXT_HUNGRY, &gameContext.hungry);
//   StrWrite(TEXT_SAD, &gameContext.sad);
//
//   SetConfigFlags(FLAG_FULLSCREEN_MODE);
//
//   int screenWidth = config->screenWidth;
//   int screenHeight = config->screenHeight;
//   const char *title = config->gameTitle;
//
//   InitWindow(screenWidth, screenHeight, title);
//   SetExitKey(KEY_Q);
//
//   int width = GetRenderWidth();
//   int height = GetRenderHeight();
//   Vector2 offset = {(int)(width / 2), (int)(height / 2)};
//
//   Camera2D camera = {.offset = offset, .zoom = 1.0};
//   SetTargetFPS(60);
//   Input input;
//
//   gameContext.slimeTexture_idle = LoadTexture("./art/blug_idle.png");
//   gameContext.slimeTexture_happy = LoadTexture("./art/blug_happy.png");
//   gameContext.slimeTexture_sad = LoadTexture("./art/blug_sad.png");
//
//   gameContext.currentSlimeTexture = gameContext.slimeTexture_idle;
//
//   gameContext.source = (Rectangle){
//       .x = 0,
//       .y = 0,
//       .width = gameContext.currentSlimeTexture.width,
//       .height = gameContext.currentSlimeTexture.height,
//   };
//
//   gameContext.destination = (Rectangle){
//       .x = -gameContext.currentSlimeTexture.width * 4,
//       .y = -gameContext.currentSlimeTexture.height * 4,
//       .width = gameContext.source.width * 8,
//       .height = gameContext.source.height * 8,
//   };
//
//   gameContext.rotation = 0;
//   gameContext.origin = (Vector2){0, 0};
//   gameContext.tint = WHITE;
//
//   SetTextureFilter(gameContext.currentSlimeTexture, TEXTURE_FILTER_POINT);
//
//   float timer;
//   timer = 0;
//   float tickRate;
//   tickRate = 5;
//
//   Command command = {.kind = CMD_UPDATE_TEXTURE,
//                      .data.updateTexture = {TEXTURE_SLIME_IDLE}};
//   RingCommandPush(&ring, command);
//
//   ButtonGroup btn_grp = CreateButtonGroup();
//
//   while (!WindowShouldClose()) {
//     InputPull(&input, camera);
//
//     float dt = GetFrameTime();
//
//     timer += dt;
//     if (timer >= tickRate) {
//       Tick(&ring);
//       timer = 0;
//     }
//
//     UIUpdate(&ring, &input, &btn_grp);
//     GameUpdate(&ring, &input, dt);
//     RingFlush(&ring, execute, &gameContext);
//
//     RenderUpdate(&gameContext, &input, camera);
//     UIRender(&btn_grp);
//   }
//
//   UnloadTexture(gameContext.slimeTexture_idle);
//   UnloadTexture(gameContext.slimeTexture_happy);
//   UnloadTexture(gameContext.slimeTexture_sad);
//
//   CloseWindow();
// }

/* vim:set ts=3 sw=2 sts=2 et: */
