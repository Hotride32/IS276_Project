#include "companion.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"
#include "pickup.h"
#include "dagger.h"
//#include "particle_effects.h"
#include "entity.h"

//void companion_draw(Entity *self);
void companion_think(Entity *self);
void companion_update(Entity *self);
int  companion_touch(Entity *self,Entity *other);
int  companion_damage(Entity *self,int amount, Entity *source);
void companion_die(Entity *self);
int companion_player_sight_check(Entity *self);
void companion_think_hunting(Entity *self);
void companion_turn(Entity *self,int dir);
void companion_think_patroling(Entity *self);

//Entity *companion_new(Vector2D position,char *actorFile);

Entity *companion_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/AL-removebg-preview (1).png",64,56,6);
    ent->sprite = gf2d_sprite_load_all("images/healthfairy.png", 16, 16, 1);
    

    

    vector2d_copy(ent->position,position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    //ent->think = companion_think_patroling;
    ent->think = companion_think_hunting;
    //ent->draw = companion_draw;
    ent->update = companion_update;
    //ent->touch = companion_touch;
    ent->damage = companion_damage;
    //ent->die = companion_die;
    ent->health = 10;
    ent->maxHealth = 10;
    ent->str = 0;
    ent->attackcool = 20;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    vector2d_set(ent->flip, 0, 0);
    vector2d_set(ent->facing, -1, 0);
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "companion",
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
    //return companion_new(position,"actors/space_bug.actor");
}
Entity* mpFairy_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/AL-removebg-preview (1).png",64,56,6);
    ent->sprite = gf2d_sprite_load_all("images/magicfairy.png", 16, 16, 1);




    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    //ent->think = companion_think_patroling;
    ent->think = companion_think_hunting;
    //ent->draw = companion_draw;
    ent->update = companion_update;
    //ent->touch = companion_touch;
    ent->damage = companion_damage;
    //ent->die = companion_die;
    ent->health = 10;
    ent->maxHealth = 10;
    ent->str = 1;
    ent->attackcool = 20;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    vector2d_set(ent->flip, 0, 0);
    vector2d_set(ent->facing, -1, 0);
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "companion",
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
    //return companion_new(position,"actors/space_bug.actor");
}
Entity* daggerFairy_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/AL-removebg-preview (1).png",64,56,6);
    ent->sprite = gf2d_sprite_load_all("images/daggerfairy.png", 16, 16, 1);




    vector2d_copy(ent->position, position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    //ent->think = companion_think_patroling;
    ent->think = companion_think_hunting;
    //ent->draw = companion_draw;
    ent->update = companion_update;
    //ent->touch = companion_touch;
    ent->damage = companion_damage;
    //ent->die = companion_die;
    ent->health = 10;
    ent->maxHealth = 10;
    ent->str = 2;
    ent->attackcool = 20;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    vector2d_set(ent->flip, 0, 0);
    vector2d_set(ent->facing, -1, 0);
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "companion",
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
    //return companion_new(position,"actors/space_bug.actor");
}


/*Entity *companion_new(Vector2D position,char *actorFile)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    //gf2d_line_cpy(self->name,"companion");
    //self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-32, -10, 60, 35);
    gf2d_body_set(
        &self->body,
        "companion",
        1,//world layer
        companion_LAYER,
        0,
        2,
        position,
        vector2d(0,0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL);

    //gf2d_actor_load(&self->actor,actorFile);
    //gf2d_actor_set_action(&self->actor,"walk");
    
    //self->sound[0] = gf2d_sound_load("sounds/bug_attack1.wav",0.1,-1);
    //self->sound[1] = gf2d_sound_load("sounds/bug_pain1.wav",0.1,-1);

    
    vector2d_copy(self->position,position);
    
   // vector2d_copy(self->scale,self->actor.al->scale);
    //vector2d_set(self->scaleCenter,64,64);
    vector3d_set(self->rotation,64,64,0);
    vector2d_set(self->flip,0,0);
    vector2d_set(self->facing,-1,0);
    
    self->think = companion_think_patroling;
    self->draw = companion_draw;
    self->update = companion_update;
    self->touch = companion_touch;
    self->damage = companion_damage;
    self->die = companion_die;
    //self->free = level_remove_entity(self);

    self->health = self->maxHealth = 20;
    level_add_entity(self);
    return self;
}
*/
/*void companion_draw(Entity *self)
{
    
}*/

void companion_think_attacking(Entity *self)
{


    
    //slog("gained sight of the player");
    if (self->attackcool == 0) {



        Shape s;
        int i, count;
        Entity* other;
        Collision* c;
        List* collisionList = NULL;
        //s = gf2d_shape_rect(self->position.x, self->position.y, self->position.x + 42 , self->position.y + 42);
        s = gf2d_body_to_shape(&self->body);
        collisionList = entity_get_clipped_entities(self, s, PLAYER_LAYER, 0);
        count = gfc_list_get_count(collisionList);
        slog("hit %i targets", count);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->body)continue;
            if (!c->body->data)continue;
            other = c->body->data;
            if (other->damage)other->damage(other, 50, self);//TODO: make this based on weapon / player stats
        }
        gf2d_collision_list_free(collisionList);

        self->think = companion_think_hunting;

        self->attackcool = 15;
    }
    
}


void companion_attack(Entity *self)
{
    //slog("attacking player");
    //gf2d_actor_set_action(&self->actor,"attack1");
    //self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
    self->think = companion_think_attacking;
    //gf2d_sound_play(self->sound[1],0,1,-1,-1);
}

void companion_think_patroling(Entity *self)
{
    self->frameAnimStart = 0;
    if (self->frame > 4) {
        self->frame = 0;
    }
    //i++;
    self->frameCount = 4;
    //slog("think patrolling");
    if (companion_player_sight_check(self))
    {
        self->think = companion_think_hunting;
        return;
    }
    /*if ((!entity_platform_end_check(self))||entity_wall_check(self, vector2d(3 *self->facing.x,0)))
    {
        companion_turn(self,self->facing.x * -1);
    }*/
    
    
    //Entity* player = player_get();

    if (self->spawnPos.x - 20 > self->position.x)
    {
        companion_turn(self, 1);

    }
    if (self->spawnPos.x + 20 < self->position.x)
    {
        companion_turn(self, -1);

    }


    //slog("Health : %0.0f", 1 * self->facing.x);
    self->velocity.x = 1 * self->facing.x;
    //self->velocity.y = sin(1 * self->facing.y);
    
}

void companion_turn(Entity *self,int dir)
{
    if (dir < 0)
    {
        self->facing.x = -1;
        self->flip.x = 1;
    }
    else if (dir > 0)
    {
        self->facing.x = 1;
        self->flip.x = 0;
    }
}

void companion_think_hunting(Entity *self)
{
    Entity *player = player_get();
    //slog("think hunting");
    //self->attackcool += 1;

    if (player->position.x < self->position.x)
    {
        companion_turn(self,-1);
        self->velocity.x = -0.75;
    }
    if (player->position.x > self->position.x)
    {
        companion_turn(self,1);
        self->velocity.x = 0.75;
    }

    if (player->position.y > self->position.y)
    {
        //companion_turn(self, -1);
        self->velocity.y = 0.75;
    }
    if (player->position.y < self->position.y)
    {
        //companion_turn(self, 1);
        self->velocity.y = -0.75;
    }
    if (self->attackcool == 0) {
        if (self->str == 0) {
            health_spawn(self->position);
        }
        else if (self->str == 1) {
            magic_spawn(self->position);
        }
        else if (self->str == 2) {
            dagger_spawn(self->position,self->flip);
        }
        self->attackcool = 200;
    }
}

void companion_think(Entity *self)
{
    if (companion_player_sight_check(self))
    {
        self->think = companion_think_hunting;
    }
}

void companion_update(Entity *self)
{
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    if (self->attackcool > 0) self->attackcool -= 0.2;
    else self->attackcool = 0;
    //world clipping
    if (self->cooldown > 0) self->cooldown--;
    if (self->cooldown < 0)self->cooldown = 0;

    
    // walk dampening
    if (self->velocity.x)
    {
        self->velocity.x *= 0.8;
        if (fabs(self->velocity.x) < 1)self->velocity.x = 0;
    }
    
    //entity_apply_gravity(self);
    entity_world_snap(self);    // error correction for collision system

    //self->velocity.y += .5;
}

int  companion_touch(Entity *self,Entity *other)
{
    slog("companion touch called");
    if (!other)return 0;
    //if (gf2d_line_cmp(other->name,"player") != 0)return 0;
    //entity_damage(other,self,5,5);
    entity_push(self,other,5);
    return 0;
}

int  companion_damage(Entity *self,int amount, Entity *source)
{
    return 0;
}

int companion_player_sight_check(Entity *self)
{
    Entity *player = player_get();
    if (!player)return 0;
    //slog("Health : %0.0f", vector2d_magnitude_compare(vector2d(self->position.x - player->position.x, self->position.y - player->position.y), 100));
    if (vector2d_magnitude_compare(vector2d(self->position.x - player->position.x,self->position.y - player->position.y),100) < 0)
    {
        //slog("found player");
        //gf2d_sound_play(self->sound[0],0,1,-1,-1);
        return 1;
    }
    return 0;
}

void companion_die(Entity *self)
{
    int r = rand() % 4;
    //slog("%i", r);
    if (r == 3) {
        knifePick_spawn(self->position);
    }
    else if (r == 2) {
        health_spawn(self->position);
    }
    else if (r == 1) {
        magic_spawn(self->position);
    }

    level_remove_entity(self);
    entity_free(self);
    /*if (!gf2d_actor_get_frames_remaining(&self->actor))
    {
        self->dead = 1;
    }*/
}
/*eol@eof*/
