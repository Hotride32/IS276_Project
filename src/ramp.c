#include "simple_logger.h"

#include "ramp.h"
#include "camera.h"
#include "gf2d_particles.h"
#include "level.h"

//static Entity* _player = NULL;

//void ramp_update(Entity *self);

//void ramp_think(Entity* self);
/*
Entity* player_get()
{
    return _player;
}
*/

Entity *ramp_spawn(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the ramp");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/tile_castle.png", 32, 32, 15);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 27;
    ent->frame = 27;
    ent->frameRate = 0.1;
    ent->frameCount = 28;
    //ent->update = ramp_update;
   // ent->think = ramp_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_edge(position.x-5, position.y+32, position.x+32, position.y-2);
    
    Level game = level_get();
    gf2d_space_add_static_shape(game.space, ent->shape);
    /*
    gf2d_body_set(
        &ent->body,
        "ramp",
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
    return ent;
}


void ramp_update(Entity *self)
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

void ramp_think(Entity* self)
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
