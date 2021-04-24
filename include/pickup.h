#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "entity.h"
#include "simple_json.h"

Entity *health_spawn(Vector2D position);

Entity* magic_spawn(Vector2D position);

Entity* bombPick_spawn(Vector2D position);

Entity* knifePick_spawn(Vector2D position);

Entity* axePick_spawn(Vector2D position);

Entity* boots_spawn(Vector2D position);

Entity* shield_spawn(Vector2D position);

Entity* sword_spawn(Vector2D position);

Entity* hp_spawn(Vector2D position);

Entity* mp_spawn(Vector2D position);

#endif
