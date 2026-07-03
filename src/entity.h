#ifndef ENTITY_H
#define ENTITY_H

#define ENTITY_MAX 1024

typedef enum {
  TRAITS_NIL = NIL,
  TRAITS_POSITIONABLE = (1 << 0),
  TRAITS_PHYSICS = (1 << 1),
  TRAITS_TEXT = (1 << 2),
} TraitFlag;

typedef u32 EntityID;
typedef u8 Traits;

typedef struct World World;

struct World {
  EntityID nextID;
  Traits traits[ENTITY_MAX];

  float dx[ENTITY_MAX];
  float dy[ENTITY_MAX];

  float x[ENTITY_MAX];
  float y[ENTITY_MAX];

  bool active[ENTITY_MAX];
};

#endif

// /* vim:set ts=3 sw=2 sts=2 et: */
