#ifndef __DAGGER_H__
#define __DAGGER_H__

#include "entity.h"

/**
 * @brief spawn a player entity
 * @param position the screen position to spawn the player at
 * @return NULL on error, or a pointer to a new player entity
 */
Entity* Dagger_spawn(Vector2D position);

#endif