#include "door.h"
#include "player.h"
#include "level.h"
#include "simple_logger.h"
#include "entity.h"
#include "dagger.h"
#include "pickup.h"
#include "camera.h"

void door_draw(Entity *self);
void door_think(Entity *self);
void door_update(Entity *self);
void door2_update(Entity* self);
void door3_update(Entity* self);
void door4_update(Entity* self);
void door_activate(Entity *self,Entity *activator);
int  door_touch(Entity *self,Entity *other);
int  door_damage(Entity *self,int amount, Entity *source);
void door_die(Entity *self);

Entity *door_new(Vector2D position);
/*
Entity *door_spawn(Vector2D position)
{
    int id = 0;
    //if (!args)return NULL;
    //sj_get_integer_value(sj_object_get_value(args,"targetId"),&id);
    return door_new(position);
}
*/

SJson* door_to_json(Entity* door)
{
    SJson* json;

    //PlayerData* pd = NULL;
    if (!door)return NULL;

    json = sj_object_new();
    if (!json)return NULL;

    //pd = (PlayerData*)player->data;
    //vector2d_copy(pd->position, player->position);

    sj_object_insert(json, "name", sj_new_str("door"));
    sj_object_insert(json, "position_x", sj_new_float(door->position.x));
    sj_object_insert(json, "position_y", sj_new_float(door->position.y));



    return json;
}

Entity* door_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the door");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/portalRings1.png", 32, 32, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 16;
    //ent->update = ramp_update;
   // ent->think = ramp_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //ent->shape = gf2d_shape_edge(position.x - 5, position.y + 32, position.x + 32, position.y - 2);

    ent->health = 1;
    ent->maxHealth = 1;

    ent->think = door_think;
    ent->save = door_to_json;
    //ent->draw = door_draw;
    ent->update = door_update;
    ent->touch = door_touch;
    ent->damage = door_damage;
    ent->die = door_die;
    ent->activate = door_activate;
    //self->free = level_remove_entity;

    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    gf2d_body_set(
        &ent->body,
        "door",
        1,
        WORLD_LAYER,
        0,
        0,
        position,
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);

    //Level game = level_get();
    //gf2d_space_add_static_shape(game.space, ent->shape);
    level_add_entity(ent);
    return ent;
}
Entity* door2_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the door");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/portalRings1.png", 32, 32, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 16;
    //ent->update = ramp_update;
   // ent->think = ramp_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //ent->shape = gf2d_shape_edge(position.x - 5, position.y + 32, position.x + 32, position.y - 2);

    ent->health = 1;
    ent->maxHealth = 1;

    ent->think = door_think;
    ent->save = door_to_json;
    //ent->draw = door_draw;
    ent->update = door2_update;
    ent->touch = door_touch;
    ent->damage = door_damage;
    ent->die = door_die;
    ent->activate = door_activate;
    //self->free = level_remove_entity;

    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    gf2d_body_set(
        &ent->body,
        "door",
        1,
        WORLD_LAYER,
        0,
        0,
        position,
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);

    //Level game = level_get();
    //gf2d_space_add_static_shape(game.space, ent->shape);
    level_add_entity(ent);
    return ent;
}

Entity* door3_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the door");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/portalRings1.png", 32, 32, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 16;
    //ent->update = ramp_update;
   // ent->think = ramp_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //ent->shape = gf2d_shape_edge(position.x - 5, position.y + 32, position.x + 32, position.y - 2);

    ent->health = 1;
    ent->maxHealth = 1;

    ent->think = door_think;
    ent->save = door_to_json;
    //ent->draw = door_draw;
    ent->update = door3_update;
    ent->touch = door_touch;
    ent->damage = door_damage;
    ent->die = door_die;
    ent->activate = door_activate;
    //self->free = level_remove_entity;

    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    gf2d_body_set(
        &ent->body,
        "door",
        1,
        WORLD_LAYER,
        0,
        0,
        position,
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);

    //Level game = level_get();
    //gf2d_space_add_static_shape(game.space, ent->shape);
    level_add_entity(ent);
    return ent;
}
Entity* door4_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the door");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/portalRings1.png", 32, 32, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 16;
    //ent->update = ramp_update;
   // ent->think = ramp_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    //ent->shape = gf2d_shape_edge(position.x - 5, position.y + 32, position.x + 32, position.y - 2);

    ent->health = 1;
    ent->maxHealth = 1;

    ent->think = door_think;
    ent->save = door_to_json;
    //ent->draw = door_draw;
    ent->update = door4_update;
    ent->touch = door_touch;
    ent->damage = door_damage;
    ent->die = door_die;
    ent->activate = door_activate;
    //self->free = level_remove_entity;

    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    gf2d_body_set(
        &ent->body,
        "door",
        1,
        WORLD_LAYER,
        0,
        0,
        position,
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);

    //Level game = level_get();
    //gf2d_space_add_static_shape(game.space, ent->shape);
    level_add_entity(ent);
    return ent;
}

void door_draw(Entity *self)
{
    
}

void door_activate(Entity *self,Entity *activator)
{
}

void door_think(Entity *self)
{
}

void door_update(Entity *self)
{
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    Level level = level_get();
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player = player_get();
        player_save(player, "levels/player.json");
        player = NULL;
        
        gf2d_entity_free_all();
        camera_set_position(vector2d(0, 0));
        //level = *level_load("levels/exampleLevel.json",0);
        level = *level_load("levels/region2/Level1.json", 2);
        player_spawn(vector2d(100, 439), "levels/player.json");
    }
    
}

void door2_update(Entity* self)
{
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    Level level = level_get();
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player = player_get();
        player_save(player, "levels/player.json");
        player = NULL;

        gf2d_entity_free_all();
        camera_set_position(vector2d(0, 0));
        //level = *level_load("levels/exampleLevel.json",0);
        level = *level_load("levels/region3/Level1.json", 3);
        player_spawn(vector2d(100, 439), "levels/player.json");
    }

}

void door3_update(Entity* self)
{
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    Level level = level_get();
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player = player_get();
        player_save(player, "levels/player.json");
        player = NULL;

        gf2d_entity_free_all();
        camera_set_position(vector2d(0, 0));
        //level = *level_load("levels/exampleLevel.json",0);
        level = *level_load("levels/bossroom/AlRoom.json", 4);
        player_spawn(vector2d(100, 439), "levels/player.json");
    }

}
void door4_update(Entity* self)
{
    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    Level level = level_get();
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player = player_get();
        player_save(player, "levels/player.json");
        player = NULL;

        gf2d_entity_free_all();
        camera_set_position(vector2d(0, 0));
        //level = *level_load("levels/exampleLevel.json",0);
        level = *level_load("levels/bossroom/WizRoom.json", 0);
        player_spawn(vector2d(100, 439), "levels/player.json");
    }

}

int  door_touch(Entity *self,Entity *other)
{
    return 0;
}

int  door_damage(Entity *self,int amount, Entity *source)
{
    //slog("CRUNCH");
    //self->health -= amount;
    /*
    if (self->state != ES_Idle)return 0;
    // check if maybe we are locked
    gf2d_sound_play(self->sound[0],0,1,-1,-1);
    gf2d_actor_set_action(&self->actor,"die");
    self->body.cliplayer = 0;
    self->state = ES_Dying;
    */
    return 0;
    
}

void door_die(Entity *self)
{
}
/*eol@eof*/
