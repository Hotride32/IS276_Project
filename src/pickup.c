#include "pickup.h"
#include "player.h"
#include "level.h"
#include "simple_logger.h"
#include "entity_common.h"
#include "gf2d_collision.h"

void pickup_draw(Entity *self);
void health_think(Entity *self);
void magic_think(Entity* self);
void knifePick_think(Entity* self);
void bombPick_think(Entity* self);
void axePick_think(Entity* self);
void sword_think(Entity* self);
void shield_think(Entity* self);
void mp_think(Entity* self);
void hp_think(Entity* self);
void boots_think(Entity* self);
void pickup_update(Entity *self);
int  pickup_touch(Entity *self,Entity *other);
int  pickup_damage(Entity *self,int amount, Entity *source);
void pickup_die(Entity *self);
//Entity *pickup_new(Vector2D position,const char *name, char *actorFile);

Entity* health_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 63;
    ent->frame = 63;
    ent->frameRate = 0.1;
    ent->frameCount = 64;
    ent->think = health_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "health potion",
        1,
        PICKUP_LAYER,
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

Entity* magic_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 64;
    ent->frame = 64;
    ent->frameRate = 0.1;
    ent->frameCount = 65;
    ent->think = magic_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "magic potion",
        1,
        PICKUP_LAYER,
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

Entity* bombPick_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/bombs.png", 16, 16, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    ent->think = bombPick_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "bomb pickup",
        1,
        PICKUP_LAYER,
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

Entity* knifePick_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 91;
    ent->frame = 91;
    ent->frameRate = 0.1;
    ent->frameCount = 92;
    ent->think = knifePick_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "knife pickup",
        1,
        PICKUP_LAYER,
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

Entity* axePick_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 87;
    ent->frame = 87;
    ent->frameRate = 0.1;
    ent->frameCount = 88;
    ent->think = axePick_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "axe pickup",
        1,
        PICKUP_LAYER,
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

Entity* shield_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 149;
    ent->frame = 149;
    ent->frameRate = 0.1;
    ent->frameCount = 150;
    ent->think = shield_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "shield",
        1,
        PICKUP_LAYER,
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

Entity* sword_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 94;
    ent->frame = 94;
    ent->frameRate = 0.1;
    ent->frameCount = 95;
    ent->think = sword_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "sword",
        1,
        PICKUP_LAYER,
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

Entity* hp_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 34;
    ent->frame = 34;
    ent->frameRate = 0.1;
    ent->frameCount = 35;
    ent->think = hp_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "health crown",
        1,
        PICKUP_LAYER,
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

Entity* boots_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 148;
    ent->frame = 148;
    ent->frameRate = 0.1;
    ent->frameCount = 149;
    ent->think = boots_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "boots",
        1,
        PICKUP_LAYER,
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

Entity* mp_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/roguelikeitems.png", 16, 16, 13);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);



    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 22;
    ent->frame = 22;
    ent->frameRate = 0.1;
    ent->frameCount = 23;
    ent->think = mp_think;
    //ent->draw = pickup_draw;
    ent->update = pickup_update;
    ent->touch = pickup_touch;
    ent->damage = pickup_damage;
    ent->die = pickup_die;
    ent->free = level_remove_entity;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "magic necklace",
        1,
        PICKUP_LAYER,
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




/*
Entity *pickup_spawn(Vector2D position,SJson *args)
{
    const char *item;
    if (!args)return NULL;
    item = sj_get_string_value(sj_object_get_value(args,"item"));
    if (!item)return NULL;
    // TODO: Make an item system more robust
    if (strcmp(item,"essence") == 0)
    {
        return pickup_new(position,item,"actors/essence.actor");
    }
    else if (strcmp(item,"crysalis") == 0)
    {
        return pickup_new(position,item,"actors/crysalis.actor");
    }
    return NULL;
}

Entity *pickup_new(Vector2D position,const char *name, char *actorFile)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    gf2d_line_cpy(self->name,name);
    self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-15, -15, 30, 30);
    gf2d_body_set(
        &self->body,
        (char *)name,
        1,
        PICKUP_LAYER,
        0,
        0,
        position,
        vector2d(0,0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL);

    gf2d_actor_load(&self->actor,actorFile);
    gf2d_actor_set_action(&self->actor,"idle");
    
    self->sound[0] = gf2d_sound_load("sounds/essence.wav",1,-1);
    
    vector2d_copy(self->position,position);
    
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->scaleCenter,self->actor.sprite->frame_w/2,self->actor.sprite->frame_h/2);
    vector3d_set(self->rotation,64,64,0);
    vector2d_set(self->flip,0,0);
    
    self->think = pickup_think;
    self->draw = pickup_draw;
    self->update = pickup_update;
    self->touch = pickup_touch;
    self->damage = pickup_damage;
    self->die = pickup_die;
    self->free = level_remove_entity;

    level_add_entity(self);
    return self;
}*/

void pickup_draw(Entity *self)
{
    
}

void health_think(Entity *self)
{
    Entity *player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->health == player->maxHealth) {

        }
        else if (player->maxHealth > (player->health + player->maxHealth * 0.1)) {
            player->health += player->maxHealth * 0.1;
            level_remove_entity(self);
            entity_free(self);
        }
        else {
            player->health = player->maxHealth;
            level_remove_entity(self);
            entity_free(self);
        }

        

        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}

void magic_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->magicPt == player->maxMagicPt) {

        }
        else if (player->maxMagicPt > (player->magicPt + player->maxMagicPt * 0.2)) {
            player->magicPt += player->maxMagicPt * 0.2;
            level_remove_entity(self);
            entity_free(self);
        }
        else {
            player->magicPt = player->maxMagicPt;
            level_remove_entity(self);
            entity_free(self);
        }



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}

void knifePick_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->knifeCount == player->maxknifeCount) {

        }
        /*else if (player->maxaxeCount > (player->health + player->maxaxeCount + 1)) {
            player->axeCount += player->maxaxeCount + 1;
            level_remove_entity(self);
            entity_free(self);
        }*/
        else {
            player->knifeCount +=  1;
            level_remove_entity(self);
            entity_free(self);
        }



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}

void axePick_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->axeCount == player->maxaxeCount) {

        }
        /*else if (player->maxaxeCount > (player->health + player->maxaxeCount + 1)) {
            player->axeCount += player->maxaxeCount + 1;
            level_remove_entity(self);
            entity_free(self);
        }*/
        else {
            player->axeCount +=  1;
            level_remove_entity(self);
            entity_free(self);
        }



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}

void bombPick_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->bombCount == player->maxbombCount) {

        }
        /*else if (player->maxaxeCount > (player->health + player->maxaxeCount + 1)) {
            player->axeCount += player->maxaxeCount + 1;
            level_remove_entity(self);
            entity_free(self);
        }*/
        else {
            player->bombCount += 1;
            level_remove_entity(self);
            entity_free(self);
        }



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}

void boots_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        if (player->jump >= 20) {

        }
        else {
            player->jump += 1;
            level_remove_entity(self);
            entity_free(self);
        }



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}
void shield_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player->def += 10;
        level_remove_entity(self);
        entity_free(self);



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}
void sword_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        player->str += 5;
        level_remove_entity(self);
        entity_free(self);


        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}
void hp_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
            player->maxHealth += 250;
            player->health = player->maxHealth;
            level_remove_entity(self);
            entity_free(self);



        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}
void mp_think(Entity* self)
{
    Entity* player;
    player = entity_get_touching_player(self);
    if (player != NULL)
    {
        
            player->maxMagicPt += 50;
            player->magicPt = player->maxMagicPt;
            level_remove_entity(self);
            entity_free(self);

        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        //self->dead = 1;            
    }
}


void pickup_update(Entity *self)
{
    entity_apply_gravity(self);
    self->velocity.y += .75;
    self->attackcool += 1;
    if (self->attackcool >= 3000) {
        level_remove_entity(self);
        entity_free(self);
    }
}

int  pickup_touch(Entity *self,Entity *other)
{
    return 0;
}

int  pickup_damage(Entity *self,int amount, Entity *source)
{
    return 0;
}

void pickup_die(Entity *self)
{
    
}
/*eol@eof*/
