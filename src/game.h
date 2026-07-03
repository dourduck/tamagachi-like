#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#define MAX_ENTITIES 1024

typedef struct {
  const char *gameTitle;
  int screenWidth;
  int screenHeight;
  int targetFps;
  int exitKey;
} GameConfig;

// typedef struct {
//   float x;
//   float y;
// } Entity_Position;
//
// typedef struct {
//   float dx;
//   float dy;
// } Entity_Velocity;

typedef struct {
  i32 id;
} Entity;

typedef enum {
  TRAIT_ACTIVE = (1 << 0),
  TRAIT_POSITIONABLE = (1 << 1),
  TRAIT_SLIME = (1 << 2),
} Entity_Traits;

#define MAX_TEXTURES 3

typedef enum {
  TEX_ID_SLIME_IDLE = 0,
  TEX_ID_SLIME_HAPPY = 1,
  TEX_ID_SLIME_SAD = 2,
} TextureID;

typedef struct {
  Texture textures[MAX_TEXTURES];
} TextureRegistry;

typedef struct {
  TextureID texture_id;
  float scale;
  Vector2 origin;
  Rectangle src;
  Rectangle dest;
} Entity_RenderData;

typedef enum {
  SLIME_MOOD_NEUTRAL,
  SLIME_MOOD_HUNGRY,
  SLIME_MOOD_COUNT,
} SlimeMood;

typedef struct {
  i32 boredom;
  i32 thirst;
  i32 hunger;
  SlimeMood mood;

  bool has_target;
  Vector2 target_position;

  float timer;
} SlimeData;

typedef struct World World;

struct World {
  i32 next_entity_id;

  Entity_Traits traits[MAX_ENTITIES];
  Vector2 position[MAX_ENTITIES];
  Vector2 velocity[MAX_ENTITIES];
  Entity_RenderData render_data[MAX_ENTITIES];
  SlimeData slime_data[MAX_ENTITIES];

  TextureRegistry texture_registry;
};

void TextureRegistry_Load(World *world);
void TextureRegistry_Unload(World *world);
void Entity_RenderData_Create(World *world, i32 entity_id, TextureID texture_id, float scale);

void GameRun(GameConfig *config);
void Game_Update(World *world);

World World_Create();

i32 Entity_NextID(World *world);

void Entity_Position_Set(World *world, i32 entity_id, float x, float y);
void Entity_Velocity_Set(World *world, i32 entity_id, float dx, float dy);

#endif

// // #define MAX_ENTITIES 128
// // #define NIL 0
// //
// // typedef int EntityID;
// // typedef uint_fast8_t Traits;
// //
// // typedef float VelocityX;
// // typedef float VelocityY;
// //
// // typedef float PositionX;
// // typedef float PositionY;
// //
// // // typedef char *Text;
// // typedef enum {
// //   TRAITS_NIL = NIL,
// //   TRAITS_POSITIONABLE = (1 << 0),
// //   TRAITS_PHYSICS = (1 << 1),
// //   TRAITS_TEXT = (1 << 2),
// //   TRAITS_PUCK = (1 << 3),
// //   TRAITS_CELL = (1 << 4),
// // } TraitFlag;
// //
// // typedef struct World World;
// //
// // struct World {
// //   EntityID nextID;
// //
// //   bool active[MAX_ENTITIES];
// //
// //   Traits traits[MAX_ENTITIES];
// //
// //   PositionX x[MAX_ENTITIES];
// //   PositionY y[MAX_ENTITIES];
// //
// //   VelocityX dx[MAX_ENTITIES];
// //   VelocityY dy[MAX_ENTITIES];
// // };
//
// typedef struct {
//   int water;
//   int rads;
//   int boredom;
//
//   Texture2D currentSlimeTexture;
//   Texture2D slimeTexture_idle;
//   Texture2D slimeTexture_happy;
//   Texture2D slimeTexture_sad;
//
//   Rectangle source;
//   Rectangle destination;
//   float rotation;
//   Vector2 origin;
//   Color tint;
//
//   World* world;
//   bool console;
//
//   str bored;
//   str hungry;
//   str sad;
// } GameCtx;
//
//
// World World_Create();
// //
// EntityID WorldEntity_Create(World *world);
//
// void WorldEntity_TraitsAdd(World *world, EntityID entityID, TraitFlag flag);
//
// void WorldEntity_TraitsRemove(World *world, EntityID entityID, TraitFlag
// flag);
//
// void WorldEntity_TraitsClear(World *world, EntityID entityID);
//
// bool WorldEntity_HasTrait(World *world, EntityID entityID, TraitFlag flag);
//
// void WorldEntity_Remove(World *world, EntityID entityID);
//
// void WorldEntity_SetPosition(World *world, EntityID entityID, float x, float
// y);
//
// void WorldEntity_SetVelocity(World *world, EntityID entityID, float dx,
//                              float dy);
//
// void InputPull(Input *input, Camera2D camera);
// void InputApply(Input input);
//
// void EntityVelocitySet(World *world, EntityID entityID, float dx, float dy);
// void EntityVelocityApply(World *world, EntityID entityID, float dt);
// void GameUpdate(Ring *ring, Input *input, float dt);
//
// #endif

/* vim:set ts=3 sw=2 sts=2 et: */
