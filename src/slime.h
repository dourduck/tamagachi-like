#ifndef SLIME_H
#define SLIME_H

i32 Slime_Create(World *world);
void system_update_slimes(World *world, float dt);
void spawn_slimes(World *world);
void render_slime_targets(World *world);

#endif

/* vim:set ts=3 sw=2 sts=2 et: */
