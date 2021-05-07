#include "monster.h"
#include "level.h"
#include "player.h"
#include "simple_logger.h"
#include "pickup.h"
//#include "particle_effects.h"
#include "entity.h"

//void monster_draw(Entity *self);
void monster_think(Entity *self);
void monster_update(Entity *self);
int  monster_touch(Entity *self,Entity *other);
int  monster_damage(Entity *self,int amount, Entity *source);
void monster_die(Entity *self);
int monster_player_sight_check(Entity *self);
void monster_think_hunting(Entity *self);
void monster_turn(Entity *self,int dir);
void monster_think_patroling(Entity *self);

//Entity *monster_new(Vector2D position,char *actorFile);

SJson* slime_to_json(Entity* slime)
{
    SJson* json;

    //PlayerData* pd = NULL;
    if (!slime)return NULL;

    json = sj_object_new();
    if (!json)return NULL;

    //pd = (PlayerData*)player->data;
    //vector2d_copy(pd->position, player->position);

    sj_object_insert(json, "name", sj_new_str("slime"));
    sj_object_insert(json, "position_x", sj_new_float(slime->position.x));
    sj_object_insert(json, "position_y", sj_new_float(slime->position.y));



    return json;
}

Entity *monster_spawn(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/slime-Sheet.png", 32, 25, 8);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);

    

    vector2d_copy(ent->position,position);
    vector2d_copy(ent->spawnPos, position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 4;
    ent->think = monster_think_patroling;
    //ent->think = monster_think_hunting;
    //ent->draw = monster_draw;
    ent->update = monster_update;
    ent->save = slime_to_json;
    //ent->touch = monster_touch;
    ent->damage = monster_damage;
    //ent->die = monster_die;
    ent->health = 10;
    ent->maxHealth = 10;
    ent->str = 50;
    ent->attackcool = 0;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    vector2d_set(ent->flip, 0, 0);
    vector2d_set(ent->facing, -1, 0);
    ent->shape = gf2d_shape_rect(0, 0, 32, 25);
    gf2d_body_set(
        &ent->body,
        "slime",
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
    //return monster_new(position,"actors/space_bug.actor");
}


/*Entity *monster_new(Vector2D position,char *actorFile)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    //gf2d_line_cpy(self->name,"monster");
    //self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-32, -10, 60, 35);
    gf2d_body_set(
        &self->body,
        "monster",
        1,//world layer
        MONSTER_LAYER,
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
    
    self->think = monster_think_patroling;
    self->draw = monster_draw;
    self->update = monster_update;
    self->touch = monster_touch;
    self->damage = monster_damage;
    self->die = monster_die;
    //self->free = level_remove_entity(self);

    self->health = self->maxHealth = 20;
    level_add_entity(self);
    return self;
}
*/
/*void monster_draw(Entity *self)
{
    
}*/

void monster_think_attacking(Entity *self)
{


    self->frameAnimStart = 8;
    if (self->frame < 8) {
        self->frame = 8;
    }
    //i++;
    self->frameCount = 14;
    //slog("gained sight of the player");
    if (self->frame >= 12) {



        Shape s;
        int i, count;
        Entity* other;
        Collision* c;
        List* collisionList = NULL;
        s = gf2d_shape_rect(self->position.x + (self->flip.x * -48) - 16, self->position.y, 50 , 40);
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
            if (other->damage)other->damage(other, 50, self);//TODO: make this based on weapon / player stats
        }
        gf2d_collision_list_free(collisionList);

        self->think = monster_think_hunting;

        self->attackcool = 15;
    }
    
}


void monster_attack(Entity *self)
{
    //slog("attacking player");
    //gf2d_actor_set_action(&self->actor,"attack1");
    //self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
    self->think = monster_think_attacking;
    //gf2d_sound_play(self->sound[1],0,1,-1,-1);
}

void monster_think_patroling(Entity *self)
{
    self->frameAnimStart = 0;
    if (self->frame > 4) {
        self->frame = 0;
    }
    //i++;
    self->frameCount = 4;
    //slog("think patrolling");
    if (monster_player_sight_check(self))
    {
        self->think = monster_think_hunting;
        return;
    }
    /*if ((!entity_platform_end_check(self))||entity_wall_check(self, vector2d(3 *self->facing.x,0)))
    {
        monster_turn(self,self->facing.x * -1);
    }*/
    
    
    //Entity* player = player_get();

    if (self->spawnPos.x -10 > self->position.x)
    {
        monster_turn(self, 1);

    }
    if (self->spawnPos.x + 10 < self->position.x)
    {
        monster_turn(self, -1);

    }


    //slog("Health : %0.0f", 1 * self->facing.x);
    self->velocity.x = 1 * self->facing.x;
    
}

void monster_turn(Entity *self,int dir)
{
    if (dir < 0)
    {
        self->facing.x = -1;
        self->flip.x = 0;
    }
    else if (dir > 0)
    {
        self->facing.x = 1;
        self->flip.x = 1;
    }
}

void monster_think_hunting(Entity *self)
{
    Entity *player = player_get();
    //slog("think hunting");
    self->frameAnimStart = 0;
    if (self->frame > 4) {
        self->frame = 0;
    }
    //i++;
    self->frameCount = 4;

    if ((self->jumpcool) || (self->cooldown))return;
    //monster loses sight of player
    if (vector2d_magnitude_compare(vector2d(self->position.x - player->position.x,self->position.y - player->position.y),150) > 0)
    {
        //slog("lost the player");
        self->think = monster_think_patroling;// idle think
        //gf2d_actor_set_action(&self->actor,"walk");
        return;
    }
    //monster gets in range of player
    if ((vector2d_magnitude_compare(vector2d(self->position.x - player->position.x,self->position.y - player->position.y),40) < 0) && self->attackcool == 0)
    {
        //self->frameAnimStart = 8;
        //if (self->frame < 8) {
        //    self->frame = 8;
        //}
        //i++;
        //self->frameCount = 14;
        //slog("gained sight of the player");
        //if (self->frame >= 12) {
            monster_attack(self);
        //}
        return;
    }
    //slog("moving towards player");
    // jump to player
    self->jumpcool = 20;
    self->velocity.y = -2;
    if (player->position.x+40 < self->position.x)
    {
        monster_turn(self,-1);
        self->velocity.x = -5;
    }
    if (player->position.x+40 > self->position.x)
    {
        monster_turn(self,1);
        self->velocity.x = 5;
    }
}

void monster_think(Entity *self)
{
    if (monster_player_sight_check(self))
    {
        self->think = monster_think_hunting;
    }
}

void monster_update(Entity *self)
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
    
    entity_apply_gravity(self);
    entity_world_snap(self);    // error correction for collision system

    self->velocity.y += .5;
}

int  monster_touch(Entity *self,Entity *other)
{
    slog("monster touch called");
    if (!other)return 0;
    //if (gf2d_line_cmp(other->name,"player") != 0)return 0;
    //entity_damage(other,self,5,5);
    entity_push(self,other,5);
    return 0;
}

int  monster_damage(Entity *self,int amount, Entity *source)
{
    Vector2D dir = {0};
    slog("monster taking %i damage!",amount);
    self->health -= amount;
    //gf2d_sound_play(self->sound[1],0,0.1,-1,-1);
    vector2d_sub(dir,source->position,self->position);
    vector2d_normalize(&dir);
    vector2d_scale(dir,dir,3);
    //particle_spray(self->position, dir,gf2d_color8(240,0,0,255), 100);
    if (self->health <= 0)
    {
        self->health = 0;
        self->think = monster_die;
        //gf2d_actor_set_action(&self->actor,"death1");
    }
    else
    {
        //gf2d_actor_set_action(&self->actor,"pain1");
        //self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
    }
    return amount;//todo factor in shields}
}

int monster_player_sight_check(Entity *self)
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

void monster_die(Entity *self)
{
    int r = rand() % 3;
    //slog("%i", r);
    if (r == 2) {
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
