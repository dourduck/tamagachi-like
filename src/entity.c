if
#include "raylib.h" // maybe?

#define MAX_ENTITIES 128

typedef struct World World;

struct World {
  EntityID nextID;

  bool active[MAX_ENTITIES];

  Traits traits[MAX_ENTITIES];

  PositionX x[MAX_ENTITIES];
  PositionY y[MAX_ENTITIES];

  VelocityX dx[MAX_ENTITIES];
  VelocityY dy[MAX_ENTITIES];
};


// #define MAX_ENTITIES 128
// #define NIL 0
//
// typedef int EntityID;
// typedef uint_fast8_t Traits;
//
// typedef float VelocityX;
// typedef float VelocityY;
//
// typedef float PositionX;
// typedef float PositionY;
//
// // typedef char *Text;
// typedef enum {
//   TRAITS_NIL = NIL,
//   TRAITS_POSITIONABLE = (1 << 0),
//   TRAITS_PHYSICS = (1 << 1),
//   TRAITS_TEXT = (1 << 2),
//   TRAITS_PUCK = (1 << 3),
//   TRAITS_CELL = (1 << 4),
// } TraitFlag;
//
// typedef struct World World;
//
// struct World {
//   EntityID nextID;
//
//   bool active[MAX_ENTITIES];
//
//   Traits traits[MAX_ENTITIES];
//
//   PositionX x[MAX_ENTITIES];
//   PositionY y[MAX_ENTITIES];
//
//   VelocityX dx[MAX_ENTITIES];
//   VelocityY dy[MAX_ENTITIES];
// };

// typedef struct {
// int water;
// int rads;
// int boredom;
//
// Texture2D currentSlimeTexture;
// Texture2D slimeTexture_idle;
//   Texture2D slimeTexture_happy;
//     Texture2D slimeTexture_sad;
//
//       Rectangle source;
// 	Rectangle destination;
// 	  float rotation;
// 	    Vector2 origin;
// 	      Color tint;
//
// 		World* world;
// 		  bool console;
//
// 		    str bored;
// 		      str hungry;
// 			str sad;
// 			} GameCtx;
//
//
// 			World World_Create();
// 			//
// 			EntityID WorldEntity_Create(World *world);
//
// 			void WorldEntity_TraitsAdd(World *world, EntityID entityID, TraitFlag flag);
//
// 			void WorldEntity_TraitsRemove(World *world, EntityID entityID, TraitFlag flag);
//
// 			void WorldEntity_TraitsClear(World *world, EntityID entityID);
//
// 			bool WorldEntity_HasTrait(World *world, EntityID entityID, TraitFlag flag);
//
// 			void WorldEntity_Remove(World *world, EntityID entityID);
//
// 			void WorldEntity_SetPosition(World *world, EntityID entityID, float x, float y);
//
// 			void WorldEntity_SetVelocity(World *world, EntityID entityID, float dx,
// 						     float dy);
//
// 						     void InputPull(Input *input, Camera2D camera);
// 						     void InputApply(Input input);
//
// 						     void EntityVelocitySet(World *world, EntityID entityID, float dx, float dy);
// 						     void EntityVelocityApply(World *world, EntityID entityID, float dt);
// 						     void GameUpdate(Ring *ring, Input *input, float dt);
//
// 						     #endif
// 						     // /* vim:set ts=3 sw=2 sts=2 et: */
//
