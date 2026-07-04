#define MOVE_SPEED 100.0;
#define RATE_HUNGER 10;

i32 Slime_Create(World *world) {
  i32 entity_id = Entity_NextID(world);

  SlimeData slime_data = {.boredom = 0,
                          .hunger = 30,
                          .thirst = 0,
                          .mood = SLIME_MOOD_NEUTRAL,
                          .timer = 0.0};

  world->slime_data[entity_id] = slime_data;

  i32 traits_slime =
      FLAGS_COMBINE(TRAIT_SLIME, TRAIT_ACTIVE, TRAIT_POSITIONABLE);

  world->traits[entity_id] = traits_slime;

  Entity_Position_Set(world, entity_id, 100, 100);
  Entity_RenderData_Create(world, entity_id, TEX_ID_SLIME_IDLE, 2.0);

  return entity_id;
}

void system_update_slimes(World *world, float dt) {
  // TODO: find a place to store archetype flags
  // TODO: Create bitflag helper functions to improve readablity
  i32 flags = FLAGS_COMBINE(TRAIT_SLIME, TRAIT_ACTIVE, TRAIT_POSITIONABLE);

  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (FLAG_CONTAINS(world->traits[i], flags)) {
      SlimeData *slime_data = &world->slime_data[i];

      slime_data->timer += dt;
      if (slime_data->timer > 1) {
        slime_data->timer = 0;
        slime_data->hunger += RATE_HUNGER;
        printf("SLIME %d HUNGER: %d\n", i, slime_data->hunger);
      }

      if (slime_data->hunger >= 50) {
        slime_data->mood = SLIME_MOOD_HUNGRY;
      } else {
        slime_data->mood = SLIME_MOOD_NEUTRAL;
      }

      if (slime_data->hunger >= 100) {
        world->traits[i] = FLAG_REMOVE(world->traits[i], TRAIT_ACTIVE);
      }

      switch (slime_data->mood) {
      case SLIME_MOOD_NEUTRAL:
        break;
      case SLIME_MOOD_HUNGRY:
        if (slime_data->has_target) {
          Vector2 target_pos = slime_data->target_position;
          Vector2 slime_pos = world->position[i];

          Vector2 dir_to_food = Vector2Subtract(target_pos, slime_pos);
          dir_to_food = Vector2Normalize(dir_to_food);
          dir_to_food.x *= MOVE_SPEED;
          dir_to_food.y *= MOVE_SPEED;
          dir_to_food.x *= dt;
          dir_to_food.y *= dt;

          Vector2 new_pos = Vector2Add(slime_pos, dir_to_food);
          world->position[i] = new_pos;

          if (CheckCollisionPointRec(target_pos, world->render_data->dest)) {
            Vector2 new_target_pos = {0, 0};
            slime_data->target_position = new_target_pos;
            slime_data->has_target = false;
            slime_data->hunger = 0;
          }
        }

        break;

      default:
        break;
      }
    }
  }
}

void spawn_slimes(World *world) {
  i32 slime_entity_id = Slime_Create(world);
  SlimeData *slime_data = &world->slime_data[slime_entity_id];

  Vector2 target_pos = {500, 500};
  slime_data->target_position = target_pos;
  slime_data->has_target = true;
}

void render_slime_targets(World *world) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (world->traits[i] & TRAIT_SLIME) {
      SlimeData *slime_data = &world->slime_data[i];

      DrawCircleV(slime_data->target_position, 8, ORANGE);
    }
  }
}

/*-

what next?

slimes need to update over time

state machine behaviour




-*/

/* vim:set ts=3 sw=2 sts=2 et: */
