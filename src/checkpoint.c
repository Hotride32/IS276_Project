#include "simple_logger.h"

#include "checkpoint.h"
#include "camera.h"
#include "level.h"
#include "gf2d_particles.h"

//static Entity* _player = NULL;

void checkpoint_update(Entity* self);

void checkpoint_think(Entity* self);
/*
Entity* player_get()
{
    return _player;
}
*/

Entity* checkpoint_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the checkpoint");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/tile_castle.png", 32, 32, 15);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 219;
    ent->frame = 219;
    ent->frameRate = 0.1;
    ent->frameCount = 220;
    ent->update = checkpoint_update;
    ent->think = checkpoint_think;
    //ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    //Level game = level_get();
    //gf2d_space_add_static_shape(game.space, ent->shape);
    gf2d_body_set(
        &ent->body,
        "checkpoint",
        1,
        WORLD_LAYER,
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

    level_add_entity(ent);
    return ent;
}

void checkpoint_set(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    //s = gf2d_shape_rect(self->position.x + 1, self->position.y - 1, 31, 33);
    s = gf2d_body_to_shape(&self->body);
    collisionList = entity_get_clipped_entities(self, s, PLAYER_LAYER, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->spawnPos = self->position;//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void checkpoint_update(Entity* self)
{

    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);



}

void checkpoint_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);


    checkpoint_set(self);



}