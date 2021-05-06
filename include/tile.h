#pragma once
#ifndef __TILE_H__
#define __TILE_H__

#include "entity.h"

/**
 * @brief spawn a player entity
 * @param position the screen position to spawn the player at
 * @return NULL on error, or a pointer to a new player entity
 */
Entity* tile_spawn(Vector2D position);

#endif