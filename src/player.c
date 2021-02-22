#include "simple_logger.h"

#include "player.h"

static Entity* _player = NULL;

void player_update(Entity *self);
void player_attack_light(Entity* self);
void player_attack_light2(Entity* self);
void player_attack_heavy(Entity* self);
void player_attack_slide(Entity* self);

Entity* player_get()
{
    return _player;
}


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
    //ent->laser = gf2d_sprite_load_all("images/Laser_sheet.png", 300, 100, 1);
    vector2d_copy(ent->position,position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 6;
    ent->update = player_update;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(16, 5, 30, 40);
    gf2d_body_set(
        &ent->body,
        "player",
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
    

    return ent;
}


void player_update(Entity *self)
{
    //Vector2D aimdir;
    //float angle;
    int mx,my;
    const Uint8* keys;
    if (!self)return;
    SDL_GetMouseState(&mx,&my);
    int i = 0;
    /*
    aimdir.x = mx - (self->position.x + 64);
    aimdir.y = my - (self->position.y + 64);
    angle = vector_angle(aimdir.x,aimdir.y);
    self->rotation.z = angle + 90;
    */
    //self->body.position.x = self->position.x+40;
    //self->body.position.y = self->position.y;
    
    

    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE] && (self->grounded)) {

        self->frameAnimStart = 42;
        if (self->frame < 42) {
            self->frame = 42;
        }
        self->frameCount = 45;
        
        self->velocity.y -= 0.58;
        
    }
    else if (keys[SDL_SCANCODE_D]) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 0;
        self->velocity.x = 1;
        self->velocity.y += .58;
        if (keys[SDL_SCANCODE_J]) {
            self->frameAnimStart = 77;
            if (self->frame < 77) {
                self->frame = 77;
            }
            self->frameCount = 84;
            self->velocity.x = 0;
            
        }
    }
    else if (keys[SDL_SCANCODE_A]) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 1;
        self->velocity.x = -1;
        self->velocity.y += .58;
        if (keys[SDL_SCANCODE_J]) {
            self->frameAnimStart = 77;
            if (self->frame < 77) {
                self->frame = 77;
            }
            self->frameCount = 84;
            self->velocity.x = 0;
            
        }
    }
    else if (keys[SDL_SCANCODE_S]) {
        self->frameAnimStart = 65;
        if (self->frame < 65) {
            self->frame = 65;
        }
        self->frameCount = 67;
        self->velocity.x = 0;
        self->velocity.y += .58;
        if (keys[SDL_SCANCODE_J]) {
            self->frameAnimStart = 86;
            if (self->frame < 86) {
                self->frame = 86;
            }
            self->frameCount = 90;
            if (self->flip.x == 1) {
                self->velocity.x = -0.5;
                
            }
            else {
                self->velocity.x = 0.5;
                
            }
        }   
    }
    else if ((keys[SDL_SCANCODE_J]) && !(keys[SDL_SCANCODE_S]) && !(keys[SDL_SCANCODE_A]) && !(keys[SDL_SCANCODE_D])) {
        self->frameAnimStart =15;
        if (self->frame < 15) {
            self->frame = 15;
        }
        //i++;
        self->frameCount = 23;
        self->velocity.y += .58;
        /*
        if ((keys[SDL_SCANCODE_J]) && (self->frame > 21) && (i == 1)) {
            self->frameAnimStart = 24;
                if (self->frame < 24) {
                    self->frame = 24;
                }
            self->frameCount = 27;
            i--;
        }
        */
    }
    else {
        self->frameAnimStart = 0;
        self->frameCount = 6;
        self->velocity.x = 0;
        self->velocity.y += .58;
    }
}


void player_attack_light(Entity* self)
{

}

void player_attack_light2(Entity* self)
{

}

void player_attack_heavy(Entity* self)
{

}

void player_attack_slide(Entity* self)
{

}

/**/
