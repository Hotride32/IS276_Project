#include "simple_logger.h"

#include "player.h"

void player_update(Entity *self);

Entity *player_spawn(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    //ent->sprite = gf2d_sprite_load_all("images/ed210_top.png",128,128,16);
    ent->sprite = gf2d_sprite_load_all("images/Warrior_SheetnoEffect.png", 69, 44, 6);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 6;
    ent->update = player_update;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    return ent;
}


void player_update(Entity *self)
{
    //Vector2D aimdir;
    //float angle;
    //int mx,my;
    const Uint8* keys;
    if (!self)return;
    /*SDL_GetMouseState(&mx,&my);
    aimdir.x = mx - (self->position.x + 64);
    aimdir.y = my - (self->position.y + 64);
    angle = vector_angle(aimdir.x,aimdir.y);
    self->rotation.z = angle + 90;
    */
    

    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
       /*
        self->frameAnimStart = 42;
        if (self->frame < 42) {
            self->frame = 42;
        }
        self->frameCount = 45;
        
        self->velocity.y -= 0.1;
        */
    }
    else if (keys[SDL_SCANCODE_D]) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 0;
        self->velocity.x = 1;
    }
    else if (keys[SDL_SCANCODE_A]) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 1;
        self->velocity.x = -1;
    }
    else if (keys[SDL_SCANCODE_S]) {
        self->frameAnimStart = 69;
        if (self->frame < 69) {
            self->frame = 69;
        }
        self->frameCount = 73;
        if (self->flip.x == 1) {
            self->velocity.x = -1.5;
        }
        else {
            self->velocity.x = 1.5;
        }
    }
    else {
        self->frameAnimStart = 0;
        self->frameCount = 6;
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
}

/**/
