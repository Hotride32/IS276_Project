#ifndef __DAGGER_H__
#define __DAGGER_H__

#include "entity.h"

/**
 * @brief spawn a dagger entity
 * @param position the screen position to spawn the player at
 * @return NULL on error, or a pointer to a new player entity
 */
Entity* dagger_spawn(Vector2D position, Vector2D flip);

Entity* axe_spawn(Vector2D position, Vector2D flip);

Entity* fireball_spawn(Vector2D position, Vector2D flip, char layer);

Entity* barrier_spawn(Vector2D position, Vector2D flip);

Entity* bomb_spawn(Vector2D position, Vector2D flip);

Entity* laser_spawn(Vector2D position, Vector2D flip);

#endif