#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef i8 b8;
typedef i32 b32;
#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

#define PT_BEGIN(cmd)                                                          \
  switch ((cmd)->line) {                                                       \
  case 0:
#define PT_YIELD(cmd)                                                          \
  do {                                                                         \
    (cmd)->line = __LINE__;                                                    \
    return CMD_RUNNING;                                                        \
  case __LINE__:;                                                              \
  } while (0)
#define PT_END(cmd)                                                            \
  }                                                                            \
  return CMD_DONE;

#define PT_AWAIT(cmd, child_ptr, dt)                                           \
  while (call((child_ptr), execute, (dt)) == CMD_RUNNING) {                    \
    PT_YIELD(cmd);                                                             \
  }

/* vvv Arena vvv */
typedef struct Arena {
  unsigned char *base;
  size_t capacity;
  size_t offset;
} Arena;

// arena_init : [(Arena *), size_t] -> None
void arena_init(Arena *arena, size_t capacity) {
  arena->base = malloc(capacity);
  arena->capacity = capacity;
  arena->offset = 0;
}

// arena_reset : (Arena *) -> None
void arena_reset(Arena *arena) {
  arena->offset = 0; // no frees apparently, rewind and write over I guess.
}

// arena_alloc : [(Arena *), size_t, size_t] -> (void *)
void *arena_alloc(Arena *arena, size_t size, size_t align) {
  size_t cursor = (size_t)(arena->base + arena->offset);
  size_t aligned = (cursor + (align - 1)) & ~(align - 1);
  size_t new_offset = (aligned - (size_t)arena->base) + size;

  if (new_offset > arena->capacity) {
    return NULL; // lol buy more RAM
  }

  arena->offset = new_offset;

  return (void *)aligned;
}

/* Not necessary because OS can handle it but
 * feels cozy to do before closing program. */
// void arena_free(Arena *arena) { free(arena->base); }
/* ^^^ Arena ^^^ */

/* vvv Helper Macros vvv */
#define new(type, vt, ...) ((type){.base = {.vtable = &(vt)}, __VA_ARGS__})

#define call(obj, method, ...)                                                 \
  ((obj)->vtable->method((void *)(obj), ##__VA_ARGS__))

#define upcast(obj) (&(obj).base)

#define property(type, field) (((type *)self)->field)
/* ^^^ Helper Macros ^^^ */

/* vvv Command vvv */
typedef enum { CMD_DONE, CMD_RUNNING } CommandStatus;

typedef struct CommandVTable {
  CommandStatus (*execute)(void *self, float dt);
} CommandVTable;

typedef struct Command {
  const CommandVTable *vtable;
} Command;

// vtable's execute now returns CommandStatus instead of void

typedef struct {
  Command base;
  int line; // protothread resume point
  float move_speed;
  Vector2 pos;
  Vector2 target_pos;
} Command_Walk;

typedef struct {
  Command base;
  int line;
  Command_Walk walk_0;
  Command_Walk walk_1;
  Vector2 startPos;
  Vector2 waypoint;
  Vector2 finalPosition;
} Command_Patrol;

/* ^^^ Command ^^^ */

float walk_distance(Command_Walk *cmd) {
  return Vector2Distance(cmd->pos, cmd->target_pos);
}

CommandStatus command_walk_execute(void *self, float dt) {
  Command_Walk *cmd = self;

  PT_BEGIN(cmd);

  while (walk_distance(cmd) > 4.0f) {
    Vector2 dir = Vector2Normalize(Vector2Subtract(cmd->target_pos, cmd->pos));
    cmd->pos = Vector2Add(cmd->pos, Vector2Scale(dir, cmd->move_speed * dt));

    DrawCircleV(cmd->target_pos, 16, RED);
    DrawCircleV(cmd->pos, 32, GREEN);

    PT_YIELD(cmd);
  }

  PT_END(cmd);
}

static const CommandVTable command_walk_vt = {command_walk_execute};

#define Command_Walk_Create(...) new(Command_Walk, command_walk_vt, __VA_ARGS__)

/* ^^^ Command Walk ^^^ */

/* vvv Command Patrol vvv */
CommandStatus command_patrol_execute(void *self, float dt) {
  Command_Patrol *cmd = self;
  PT_BEGIN(cmd);

  cmd->walk_0 =
      Command_Walk_Create(.pos = cmd->startPos, .target_pos = cmd->waypoint,
                          .move_speed = 100.0f);
  PT_AWAIT(cmd, upcast(cmd->walk_0), dt)

  cmd->walk_1 = Command_Walk_Create(.pos = cmd->waypoint,
                                    .target_pos = cmd->finalPosition,
                                    .move_speed = 100.0f);
  PT_AWAIT(cmd, upcast(cmd->walk_1), dt)

  PT_END(cmd);
}

static const CommandVTable command_patrol_vt = {command_patrol_execute};

#define Command_Patrol_Create(...)                                             \
  new(Command_Patrol, command_patrol_vt, __VA_ARGS__)

typedef struct {
  Command base;
  int line;
  Command_Patrol patrols[3];
  bool done[3];
} Command_Patrol_Group;

CommandStatus command_patrol_group_execute(void *self, float dt) {
  Command_Patrol_Group *cmd = self;
  PT_BEGIN(cmd);

  cmd->patrols[0] =
      Command_Patrol_Create(.startPos = {.x = 400, .y = 500},
                            .waypoint = {.x = 100, .y = 100},
                            .finalPosition = {.x = 600, .y = 250});

  cmd->patrols[1] =
      Command_Patrol_Create(.startPos = {.x = 100, .y = 100},
                            .waypoint = {.x = 600, .y = 250},
                            .finalPosition = {.x = 400, .y = 500});

  cmd->patrols[2] =
      Command_Patrol_Create(.startPos = {.x = 600, .y = 250},
                            .waypoint = {.x = 400, .y = 500},
                            .finalPosition = {.x = 100, .y = 100});

  cmd->done[0] = cmd->done[1] = cmd->done[2] = false;

  for (;;) {
    bool all_done = true;

    for (int i = 0; i < 3; i++) {
      if (cmd->done[i]) {
        continue;
      }
      if (call(upcast(cmd->patrols[i]), execute, dt) == CMD_DONE) {
        cmd->done[i] = true;
      } else {
        all_done = false;
      }
    }

    if (all_done) {
      break;
    }

    PT_YIELD(cmd);
  }

  PT_END(cmd);
}

static const CommandVTable command_patrol_group_vt = {
    command_patrol_group_execute};

#define Command_Patrol_Group_Create(...)                                       \
  new(Command_Patrol_Group, command_patrol_group_vt, __VA_ARGS__)

/* ^^^ Command Patrol ^^^ */

typedef struct {
  Arena frame_arena;     // transient, reset every tick
  Command **frame_items; // one-shot commands allocated this tick
  size_t frame_count;
  size_t frame_capacity;

  Command **active; // multi-frame commands still CMD_RUNNING
  size_t active_count;
  size_t active_capacity;
} CommandBuffer;

bool cmdbuf_init(CommandBuffer *buf, size_t arena_bytes, size_t max_commands) {
  arena_init(&buf->frame_arena, arena_bytes);
  if (!buf->frame_arena.base) {
    return false;
  }

  buf->frame_items = malloc(sizeof(Command *) * max_commands);
  buf->active = malloc(sizeof(Command *) * max_commands);
  if (!buf->frame_items || !buf->active) {
    free(buf->frame_arena.base);
    free(buf->frame_items);
    free(buf->active);
    return false;
  }

  buf->frame_count = 0;
  buf->frame_capacity = max_commands;
  buf->active_count = 0;
  buf->active_capacity = max_commands;

  return true;
}

// Macro to push a one-shot command of any type/size into the buffer.
// Builds a compound literal, then copies it into frame_arena. Storage
// is only valid until the next arena_reset (i.e. the current tick).
#define cmdbuf_push(buf, type, vt, ...)                                        \
  do {                                                                         \
    type _tmp = new(type, vt, __VA_ARGS__);                                    \
    type *_slot =                                                              \
        arena_alloc(&(buf)->frame_arena, sizeof(type), _Alignof(type));        \
    memcpy(_slot, &_tmp, sizeof(type));                                        \
    (buf)->frame_items[(buf)->frame_count++] = upcast(*_slot);                 \
  } while (0)

// Macro to push a multi-frame (coroutine-style) command. Individually
// heap-allocated since these are typically few and outlive a single tick;
// freed either when it reaches CMD_DONE (in cmdbuf_tick) or at
// cmdbuf_destroy if still running at shutdown.
#define cmdbuf_push_coroutine(buf, type, vt, ...)                              \
  do {                                                                         \
    type *_slot = malloc(sizeof(type));                                        \
    *_slot = new(type, vt, __VA_ARGS__);                                       \
    (buf)->active[(buf)->active_count++] = upcast(*_slot);                     \
  } while (0)

#define cmdbuf_push_coroutine_val(buf, type, value_expr)                       \
  do {                                                                         \
    type *_slot = malloc(sizeof(type));                                        \
    *_slot = (value_expr);                                                     \
    (buf)->active[(buf)->active_count++] = upcast(*_slot);                     \
  } while (0)

void cmdbuf_tick(CommandBuffer *buf, float dt) {
  // Run one-shot commands pushed this tick.
  for (size_t i = 0; i < buf->frame_count; i++) {
    call(buf->frame_items[i], execute, dt);
  }

  // Run multi-frame commands, compacting out anything that finished.
  size_t write = 0;
  for (size_t i = 0; i < buf->active_count; i++) {
    Command *cmd = buf->active[i];
    CommandStatus status = call(cmd, execute, dt);
    if (status == CMD_RUNNING) {
      buf->active[write++] = cmd; // still running — keep it, compact array
    } else {
      free(cmd); // CMD_DONE — reclaim its heap storage now
    }
  }
  buf->active_count = write;

  // Reset transient storage for next tick.
  arena_reset(&buf->frame_arena);
  buf->frame_count = 0;
}

void cmdbuf_destroy(CommandBuffer *buf) {
  // Frame arena: one malloc backs the whole block.
  free(buf->frame_arena.base);
  buf->frame_arena.base = NULL;
  buf->frame_arena.capacity = 0;
  buf->frame_arena.offset = 0;

  free(buf->frame_items);
  buf->frame_items = NULL;
  buf->frame_count = 0;
  buf->frame_capacity = 0;

  // Active list: each entry was individually malloc'd by
  // cmdbuf_push_coroutine and never freed unless it reached CMD_DONE.
  for (size_t i = 0; i < buf->active_count; i++) {
    free(buf->active[i]);
  }
  free(buf->active);
  buf->active = NULL;
  buf->active_count = 0;
  buf->active_capacity = 0;
}

/* ^^^ Command Buffer ^^^ */

/* vvv MAIN vvv */
int main(void) {
  InitWindow(800, 600, "game");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  CommandBuffer buf;

  /* 4KB arena, 64 commands per tick */
  if (!cmdbuf_init(&buf, KiB(4), 64)) {
    fprintf(stderr, "cmdbuf_init failed\n");
    return 1;
  }

  cmdbuf_push_coroutine_val(
      &buf, Command_Patrol_Group,
      Command_Patrol_Group_Create(
              .patrols = {
                  Command_Patrol_Create(.startPos = {.x = 400, .y = 500},
                                        .waypoint = {.x = 100, .y = 100},
                                        .finalPosition = {.x = 600, .y = 250}),
                  Command_Patrol_Create(.startPos = {.x = 100, .y = 100},
                                        .waypoint = {.x = 600, .y = 250},
                                        .finalPosition = {.x = 400, .y = 500}),
                  Command_Patrol_Create(.startPos = {.x = 600, .y = 250},
                                        .waypoint = {.x = 400, .y = 500},
                                        .finalPosition = {.x = 100,
                                                          .y = 100})}));

  cmdbuf_push_coroutine_val(
      &buf, Command_Patrol,
      Command_Patrol_Create(.startPos = {.x = 600, .y = 600},
                            .waypoint = {.x = 600, .y = 0},
                            .finalPosition = {.x = 0, .y = 600}));

  // Tick repeatedly until the coroutine actually finishes, instead of
  // a single tick (which would only advance ~0.017 units and then get
  // torn down mid-flight by cmdbuf_destroy).
  // while (buf.active_count > 0) {
  // }

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    BeginDrawing();
    ClearBackground(GRAY);

    cmdbuf_tick(&buf, dt);
    EndDrawing();
  }

  cmdbuf_destroy(&buf);

  return 0;
}
/* ^^^ MAIN ^^^ */

/* vim:set ts=3 sw=2 sts=2 et: */
