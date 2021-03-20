#include "skull.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"
#include "pickup.h"
//#include "particle_effects.h"
#include "entity.h"

//void skull_draw(Entity *self);
void skull_think(Entity *self);
void skull_update(Entity *self);
int  skull_touch(Entity *self,Entity *other);
int  skull_damage(Entity *self,int amount, Entity *source);
void skull_die(Entity *self);
int skull_player_sight_check(Entity *self);
void skull_think_hunting(Entity *self);
void skull_turn(Entity *self,int dir);
void skull_think_patroling(Entity *self);

//Entity *skull_new(Vector2D position,char *actorFile);

Entity *skull_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/AL-removebg-preview (1).png",64,56,6);
    ent->sprite = gf2d_sprite_load_all("images/skull.png", 42, 42, 4);
    

    

    vector2d_copy(ent->position,position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 15;
    ent->think = skull_think_patroling;
    //ent->think = skull_think_hunting;
    //ent->draw = skull_draw;
    ent->update = skull_update;
    //ent->touch = skull_touch;
    ent->damage = skull_damage;
    //ent->die = skull_die;
    ent->health = 10;
    ent->maxHealth = 10;
    ent->str = 50;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    vector2d_set(ent->flip, 0, 0);
    vector2d_set(ent->facing, -1, 0);
    ent->shape = gf2d_shape_rect(0, 0, 30, 30);
    gf2d_body_set(
        &ent->body,
        "skull",
        1,
        MONSTER_LAYER,
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
    //return skull_new(position,"actors/space_bug.actor");
}


/*Entity *skull_new(Vector2D position,char *actorFile)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    //gf2d_line_cpy(self->name,"skull");
    //self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-32, -10, 60, 35);
    gf2d_body_set(
        &self->body,
        "skull",
        1,//world layer
        skull_LAYER,
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
    
    self->think = skull_think_patroling;
    self->draw = skull_draw;
    self->update = skull_update;
    self->touch = skull_touch;
    self->damage = skull_damage;
    self->die = skull_die;
    //self->free = level_remove_entity(self);

    self->health = self->maxHealth = 20;
    level_add_entity(self);
    return self;
}
*/
/*void skull_draw(Entity *self)
{
    
}*/

void skull_think_attacking(Entity *self)
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

        self->think = skull_think_hunting;

        self->attackcool = 15;
    }
    
}


void skull_attack(Entity *self)
{
    //slog("attacking player");
    //gf2d_actor_set_action(&self->actor,"attack1");
    //self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
    self->think = skull_think_attacking;
    //gf2d_sound_play(self->sound[1],0,1,-1,-1);
}

void skull_think_patroling(Entity *self)
{
    self->frameAnimStart = 0;
    if (self->frame > 4) {
        self->frame = 0;
    }
    //i++;
    self->frameCount = 4;
    //slog("think patrolling");
    if (skull_player_sight_check(self))
    {
        self->think = skull_think_hunting;
        return;
    }
    /*if ((!entity_platform_end_check(self))||entity_wall_check(self, vector2d(3 *self->facing.x,0)))
    {
        skull_turn(self,self->facing.x * -1);
    }*/
    
    
    //Entity* player = player_get();

    if (self->spawnPos.x - 20 > self->position.x)
    {
        skull_turn(self, 1);

    }
    if (self->spawnPos.x + 20 < self->position.x)
    {
        skull_turn(self, -1);

    }


    //slog("Health : %0.0f", 1 * self->facing.x);
    self->velocity.x = 1 * self->facing.x;
    //self->velocity.y = sin(1 * self->facing.y);
    
}

void skull_turn(Entity *self,int dir)
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

void skull_think_hunting(Entity *self)
{
    Entity *player = player_get();
    //slog("think hunting");
    

    if ((self->jumpcool) || (self->cooldown))return;
    //skull loses sight of player
    if (vector2d_magnitude_compare(vector2d(self->position.x - player->position.x,self->position.y - player->position.y),150) > 0)
    {
        //slog("lost the player");
        self->think = skull_think_patroling;// idle think
        //gf2d_actor_set_action(&self->actor,"walk");
        return;
    }
    //skull gets in range of player
    if ((vector2d_magnitude_compare(vector2d(self->position.x - player->position.x,self->position.y - player->position.y),15) < 0) && self->attackcool == 0)
    {
        //self->frameAnimStart = 8;
        //if (self->frame < 8) {
        //    self->frame = 8;
        //}
        //i++;
        //self->frameCount = 14;
        //slog("gained sight of the player");
        //if (self->frame >= 12) {
            skull_attack(self);
        //}
        return;
    }
    //slog("moving towards player");
    // jump to player
    //self->jumpcool = 20;
    //self->velocity.y = -2;
    if (player->position.x < self->position.x)
    {
        skull_turn(self,-1);
        self->velocity.x = -0.5;
    }
    if (player->position.x > self->position.x)
    {
        skull_turn(self,1);
        self->velocity.x = 0.5;
    }

    if (player->position.y > self->position.y)
    {
        //skull_turn(self, -1);
        self->velocity.y = 0.5;
    }
    if (player->position.y < self->position.y)
    {
        //skull_turn(self, 1);
        self->velocity.y = -0.5;
    }
}

void skull_think(Entity *self)
{
    if (skull_player_sight_check(self))
    {
        self->think = skull_think_hunting;
    }
}

void skull_update(Entity *self)
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

int  skull_touch(Entity *self,Entity *other)
{
    slog("skull touch called");
    if (!other)return 0;
    //if (gf2d_line_cmp(other->name,"player") != 0)return 0;
    //entity_damage(other,self,5,5);
    entity_push(self,other,5);
    return 0;
}

int  skull_damage(Entity *self,int amount, Entity *source)
{
    Vector2D dir = {0};
    slog("skull taking %i damage!",amount);
    self->health -= amount;
    //gf2d_sound_play(self->sound[1],0,0.1,-1,-1);
    vector2d_sub(dir,source->position,self->position);
    vector2d_normalize(&dir);
    vector2d_scale(dir,dir,3);
    //particle_spray(self->position, dir,gf2d_color8(240,0,0,255), 100);
    if (self->health <= 0)
    {
        self->health = 0;
        self->think = skull_die;
        //gf2d_actor_set_action(&self->actor,"death1");
    }
    else
    {
        //gf2d_actor_set_action(&self->actor,"pain1");
        //self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
    }
    return amount;//todo factor in shields}
}

int skull_player_sight_check(Entity *self)
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

void skull_die(Entity *self)
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
