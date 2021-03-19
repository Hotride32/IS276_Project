#include "simple_logger.h"

#include "dagger.h"
#include "camera.h"
#include "level.h"
#include "gf2d_particles.h"

//static Entity* _player = NULL;

void dagger_update(Entity *self);

void dagger_think(Entity* self);

void axe_update(Entity* self);

void axe_think(Entity* self);

void fireball_update(Entity* self);

void fireball_think(Entity* self);

void barrier_update(Entity* self);

void barrier_think(Entity* self);

void bomb_update(Entity* self);

void bomb_think(Entity* self);

void laser_update(Entity* self);

void laser_think(Entity* self);

/*
Entity* player_get()
{
    return _player;
}
*/

Entity *dagger_spawn(Vector2D position,Vector2D flip)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/Ice_Dagger.png", 28, 28, 14);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 1;
    ent->update = dagger_update;
    ent->think = dagger_think;
    ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(16, 0, 30, 16);
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

void dagger_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));
    collisionList = entity_get_clipped_entities(self, s, MONSTER_LAYER, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 1, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    //self->velocity.y = 0;

    
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
   
    //fireball magic
    //vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    //vector2d_add(self->velocity, self->velocity, thrust);
    
    //axe
    //vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    //vector2d_add(self->velocity, self->velocity, thrust);
    
    //barrier magic
    //vector2d_scale(thrust, vector2d(1,0), cos(self->jumpcool)*2);
    //vector2d_add(self->velocity, self->velocity, thrust);
    
    //base
    if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);
    
    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            level_remove_entity(self);
            entity_free(self);
        }
        gf2d_collision_list_free(collisionList);
        
    }
    dagger_melee(self);
    
    if (self->jumpcool >= 30) {
        level_remove_entity(self);
        entity_free(self);
    }
    
}

Entity* axe_spawn(Vector2D position, Vector2D flip)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/axe.png", 32, 32, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 1;
    ent->frame = 1;
    ent->frameRate = 0.1;
    ent->frameCount = 2;
    ent->update = axe_update;
    ent->think = axe_think;
    ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 32, 32);
    gf2d_body_set(
        &ent->body,
        "axe",
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

void axe_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));
    collisionList = entity_get_clipped_entities(self, s, MONSTER_LAYER, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 1, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void axe_update(Entity* self)
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    //self->velocity.y = 0;


}

void axe_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);

    //fireball magic
    //vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    //vector2d_add(self->velocity, self->velocity, thrust);

    //axe
    vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    vector2d_add(self->velocity, self->velocity, thrust);

    //barrier magic
    //vector2d_scale(thrust, vector2d(1,0), cos(self->jumpcool)*2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //base
    if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);

    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            level_remove_entity(self);
            entity_free(self);
        }
        gf2d_collision_list_free(collisionList);

    }
    axe_melee(self);

    if (self->jumpcool >= 30) {
        level_remove_entity(self);
        entity_free(self);
    }

}

Entity* fireball_spawn(Vector2D position, Vector2D flip, char layer)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/fireball.png", 64, 32, 2);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 5;
    ent->frame = 5;
    ent->frameRate = 0.1;
    ent->frameCount = 6;
    ent->update = fireball_update;
    ent->think = fireball_think;
    ent->flip = flip;
    ent->hitLayer = layer;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(16, 0, 30, 16);
    gf2d_body_set(
        &ent->body,
        "fireball",
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

void fireball_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));
    collisionList = entity_get_clipped_entities(self, s, self->hitLayer, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 1, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void fireball_update(Entity* self)
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    //self->velocity.y = 0;


}

void fireball_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);

    //fireball magic
    vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    vector2d_add(self->velocity, self->velocity, thrust);

    //axe
    //vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //barrier magic
    //vector2d_scale(thrust, vector2d(1,0), cos(self->jumpcool)*2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //base
    if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);

    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            level_remove_entity(self);
            entity_free(self);
        }
        gf2d_collision_list_free(collisionList);

    }
    fireball_melee(self);

    if (self->jumpcool >= 30) {
        level_remove_entity(self);
        entity_free(self);
    }

}

Entity* barrier_spawn(Vector2D position, Vector2D flip)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/shock.png", 64, 64, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 8;
    ent->frame = 8;
    ent->frameRate = 0.1;
    ent->frameCount = 12;
    ent->update = barrier_update;
    ent->think = barrier_think;
    ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 16, 16, 30);
    gf2d_body_set(
        &ent->body,
        "barrier",
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

void barrier_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));
    collisionList = entity_get_clipped_entities(self, s, MONSTER_LAYER, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 1, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void barrier_update(Entity* self)
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    //self->velocity.y = 0;


}

void barrier_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);

    //fireball magic
    //vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    //vector2d_add(self->velocity, self->velocity, thrust);

    //axe
    //vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //barrier magic
    if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1,0), cos(self->jumpcool)*2);
        vector2d_add(self->velocity, self->velocity, thrust);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), cos(self->jumpcool) * 2);
        vector2d_add(self->velocity, self->velocity, thrust);
    }
    //base
    /*if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);*/

    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            //level_remove_entity(self);
            //entity_free(self);
        }
        gf2d_collision_list_free(collisionList);

    }
    barrier_melee(self);

    if (self->jumpcool >= 30) {
        level_remove_entity(self);
        entity_free(self);
    }

}

Entity* bomb_spawn(Vector2D position, Vector2D flip, char layer)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/bombs.png", 16, 16, 4);
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position, position);
    ent->frameAnimStart = 5;
    ent->frame = 5;
    ent->frameRate = 0.1;
    ent->frameCount = 6;
    ent->update = bomb_update;
    ent->think = bomb_think;
    ent->hitLayer = layer;
    ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "bomb",
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

void bomb_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;

    
    

    s = gf2d_shape_rect(self->position.x + (self->flip.x * -48) - 16, self->position.y, 64, 64);
    collisionList = entity_get_clipped_entities(self, s, self->hitLayer, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 20, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void bomb_update(Entity* self)
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    self->velocity.y += .5;


}

void bomb_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);

    //fireball magic
    //vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    //vector2d_add(self->velocity, self->velocity, thrust);

    //axe
    //vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //barrier magic
    
    //base
    /*if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);*/

    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            //level_remove_entity(self);
            //entity_free(self);
        }
        gf2d_collision_list_free(collisionList);

    }
    

    if (self->jumpcool >= 15) {
        bomb_melee(self);
        
        

        level_remove_entity(self);
        entity_free(self);
        
    }

}

Entity* laser_spawn(Vector2D position, Vector2D flip, char layer)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the dagger");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    //ent->sprite = gf2d_sprite_load_all("images/bombs.png", 16, 16, 4);
    ent->sprite = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    ent->flip = flip;
    
        vector2d_copy(ent->position, vector2d(position.x, position.y));
    
    
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 14;
    ent->update = laser_update;
    ent->think = laser_think;
    ent->hitLayer = layer;
    //ent->flip = flip;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(0, 0, 16, 16);
    gf2d_body_set(
        &ent->body,
        "laser",
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

void laser_melee(Entity* self)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;




    s = gf2d_shape_rect(self->position.x + (self->flip.x * -48) - 16, self->position.y, 300, 32);
    collisionList = entity_get_clipped_entities(self, s, self->hitLayer, 0);
    count = gfc_list_get_count(collisionList);
    //slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, 20, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void laser_update(Entity* self)
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
    /*
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    */
    entity_world_snap(self);    // error correction for collision system
    self->jumpcool += 0.2;
    //entity_apply_gravity(self);

    //self->velocity.y = 0;


}

void laser_think(Entity* self)
{

    const Uint8* keys;

    Vector2D aimdir, camera, thrust;
    float angle;
    int mx, my;
    if (!self)return;
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mx, &my);

    //fireball magic
    //vector2d_scale(thrust, vector2d(0, -1), sin(self->jumpcool));
    //vector2d_add(self->velocity, self->velocity, thrust);

    //axe
    //vector2d_scale(thrust, vector2d(0, -1), -0.75*abs(self->jumpcool)+2);
    //vector2d_add(self->velocity, self->velocity, thrust);

    //barrier magic

    //base
    /*if (self->flip.x == 1) {
        vector2d_scale(thrust, vector2d(-1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    else {
        vector2d_scale(thrust, vector2d(1, 0), 1.85);
        //vector2d_scale(thrust, vector2d(-1, 0), 1);
    }
    vector2d_add(self->velocity, self->velocity, thrust);*/

    Shape s;
    int i, count;
    Collision* c;
    List* collisionList;
    CollisionFilter filter = {
        1,
        WORLD_LAYER,
        0,
        0,
        &self->body
    };

    if (!self)return 0;
    s = gf2d_body_to_shape(&self->body);
    gf2d_shape_move(&s, vector2d(0.1, 0));

    collisionList = gf2d_collision_check_space_shape(level_get_space(), s, filter);
    if (collisionList != NULL)
    {
        count = gfc_list_get_count(collisionList);
        for (i = 0; i < count; i++)
        {
            c = (Collision*)gfc_list_get_nth(collisionList, i);
            if (!c)continue;
            if (!c->shape)continue;
            gf2d_shape_draw(*c->shape, gfc_color(255, 255, 0, 255), camera_get_offset());
            //level_remove_entity(self);
            //entity_free(self);
        }
        gf2d_collision_list_free(collisionList);

    }

    if (self->frame >= 9) {
        laser_melee(self);
    }
    if (self->frame >= 10) {
        level_remove_entity(self);
        entity_free(self);
    }
    //if (self->jumpcool >= 30) {
        //laser_melee(self);



     //   level_remove_entity(self);
    //    entity_free(self);

    //}

}

/**/
