#include "simple_logger.h"

#include "dagger.h"
#include "camera.h"
#include "gf2d_particles.h"

//static Entity* _player = NULL;

void dagger_update(Entity *self);

void dagger_think(Entity* self);
/*
Entity* player_get()
{
    return _player;
}
*/

Entity *dagger_spawn(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/gear_daggers.png", 48, 48, 7);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    ent->update = dagger_update;
    ent->think = dagger_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(16, 5, 30, 40);
    gf2d_body_set(
        &ent->body,
        "dagger",
        1,
        OBJECT_LAYER,
        0,
        1,
        position,
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);
    
    
    return ent;
}


void dagger_update(Entity *self)
{
    
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    
    // apply dampening on velocity
    vector2d_scale(self->velocity, self->velocity, 0.75);
    if (vector2d_magnitude_squared(self->velocity) < 2)
    {
        vector2d_clear(self->velocity);
    }
    
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;

    entity_world_snap(self);    // error correction for collision system
    //entity_apply_gravity(self);

    //self->velocity.y += .5;

    
}

void dagger_think(Entity* self)
{
    
    const Uint8* keys;
    
    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);
    /*
    camera = camera_get_position();
    mx += camera.x;
    my += camera.y;
    aimdir.x = mx - (self->position.x + 64);
    aimdir.y = my - (self->position.y + 64);
    angle = vector_angle(aimdir.x, aimdir.y);
    self->rotation.z = angle + 90;

    // turn aimdir into a unit vector
    vector2d_normalize(&aimdir);
    // check for motion
    if (keys[SDL_SCANCODE_W])
    {
        vector2d_scale(thrust, aimdir, 5);
        vector2d_add(self->velocity, self->velocity, thrust);

    }
    */

    vector2d_scale(thrust, vector2d(1,0), 1);
    vector2d_add(self->velocity, self->velocity, thrust);
    
    
}



/**/
