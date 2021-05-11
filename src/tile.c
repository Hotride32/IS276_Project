#include "simple_logger.h"

#include "tile.h"
#include "camera.h"
#include "gf2d_particles.h"
#include "level.h"

//static Entity* _player = NULL;

//void tile_update(Entity *self);

//void tile_think(Entity* self);
/*
Entity* player_get()
{
    return _player;
}
*/

SJson* tile_to_json(Entity* tile)
{
    SJson* json;
    slog("tile");
    //PlayerData* pd = NULL;
    if (!tile)return NULL;

    json = sj_object_new();
    if (!json)return NULL;

    //pd = (PlayerData*)player->data;
    //vector2d_copy(pd->position, player->position);

    sj_object_insert(json, "name", sj_new_str("tile"));
    sj_object_insert(json, "position_x", sj_new_float(tile->position.x));
    sj_object_insert(json, "position_y", sj_new_float(tile->position.y));



    return json;
}

Entity *tile_spawn(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the tile");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/basetileset.png", 32, 32, 16);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 2;
    ent->frame = 2;
    ent->frameRate = 0.1;
    ent->frameCount = 3;
    ent->save = tile_to_json;
    //ent->update = tile_update;
   // ent->think = tile_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(position.x, position.y, 32, 32);
    
    //Level game = level_get();
    gf2d_space_add_static_shape(level_get_space(), ent->shape);
    /*
    gf2d_body_set(
        &ent->body,
        "tile",
        0,
        MONSTER_LAYER,
        0,
        1,
        position,
        vector2d(0, 0),
        100,
        100,
        0,
        &ent->shape,
        ent,
        NULL);
    
    */
    //level_add_entity(ent);
    return ent;
}


void tile_update(Entity *self)
{
    
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
   /*
    // apply dampening on velocity
    vector2d_scale(self->velocity, self->velocity, 0.75);
    if (vector2d_magnitude_squared(self->velocity) < 2)
    {
        vector2d_clear(self->velocity);
    }
    
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    //entity_world_snap(self);    // error correction for collision system
    //entity_apply_gravity(self);

    //self->velocity.y += .5;

    
}

void tile_think(Entity* self)
{
    
    const Uint8* keys;
    
    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);
    

   // vector2d_scale(thrust, vector2d(1,0), 1);
   // vector2d_add(self->velocity, self->velocity, thrust);
    
    
}



/**/
