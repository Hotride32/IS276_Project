#include "simple_logger.h"

#include "player.h"
#include "camera.h"
#include "dagger.h"
#include "level.h"
#include "gf2d_particles.h"

static Entity* _player = NULL;



void player_update(Entity *self);
void player_attack_light(Entity* self);
void player_attack_light2(Entity* self);
void player_attack_heavy(Entity* self);
void player_attack_slide(Entity* self);
void player_think(Entity* self);
int  player_damage(Entity* self, int amount, Entity* source);

Entity* player_get()
{
    return _player;
}


SJson* player_to_json(Entity* player)
{
    SJson* json;
    //PlayerData* pd = NULL;
    if (!player)return NULL;

    json = sj_object_new();
    if (!json)return NULL;

    //pd = (PlayerData*)player->data;
    //vector2d_copy(pd->position, player->position);

    sj_object_insert(json, "maxHealth", sj_new_float(player->maxHealth));
    sj_object_insert(json, "str", sj_new_int(player->str));
    sj_object_insert(json, "position_x", sj_new_float(player->spawnPos.x));
    sj_object_insert(json, "position_y", sj_new_float(player->spawnPos.y));

    return json;
}

void player_save(Entity* player, char* filename)
{
    SJson* json;
    if (!player)return;

    json = player_to_json(player);
    if (!json)return;
    sj_save(json, filename);
}


Entity *player_spawn(Vector2D position, const char* filename)
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

    SJson* json;
    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load scene file %s", filename);
        entity_free(ent);
        return NULL;
    }
    
    sj_get_float_value(sj_object_get_value(json, "maxHealth"), &ent->maxHealth);
    sj_get_float_value(sj_object_get_value(json, "position_x"), &ent->position.x);
    sj_get_float_value(sj_object_get_value(json, "position_y"), &ent->position.y);
    
    //vector2d_copy(ent->position,position);
    ent->frameAnimStart = 0;
    ent->frame = 0;
    ent->frameRate = 0.1;
    ent->frameCount = 6;
    ent->update = player_update;
    ent->think = player_think;
    ent->damage = player_damage;
    ent->health = ent->maxHealth;
    ent->maxHealth = 1000;
    ent->str = 5;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->shape = gf2d_shape_rect(16, 5, 30, 30);
    gf2d_body_set(
        &ent->body,
        "player",
        1,
        PLAYER_LAYER,
        0,
        1,
        vector2d(ent->position.x,ent->position.y),
        vector2d(0, 0),
        10,
        1,
        0,
        &ent->shape,
        ent,
        NULL);
    

    return ent;
}

void player_melee(Entity* self,float damage)
{
    Shape s;
    int i, count;
    Entity* other;
    Collision* c;
    List* collisionList = NULL;
    s = gf2d_shape_rect(self->position.x + (self->flip.x * -48) + 16, self->position.y, 32, 40);
    collisionList = entity_get_clipped_entities(self, s, MONSTER_LAYER, 0);
    count = gfc_list_get_count(collisionList);
    slog("hit %i targets", count);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        other = c->body->data;
        if (other->damage)other->damage(other, damage, self);//TODO: make this based on weapon / player stats
    }
    gf2d_collision_list_free(collisionList);
}

void player_update(Entity *self)
{
    
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    cameraSize = camera_get_dimensions();
    camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
    camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
    camera_set_position(camera);
    // apply dampening on velocity
    vector2d_scale(self->velocity, self->velocity, 0.75);
    if (vector2d_magnitude_squared(self->velocity) < 2)
    {
        vector2d_clear(self->velocity);
    }
    
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    if (self->projectcool > 0) self->projectcool -= 0.2;
    else self->projectcool = 0;
    if (self->attackcool > 0) self->attackcool -= 0.2;
    else self->attackcool = 0;

    entity_world_snap(self);    // error correction for collision system
    entity_apply_gravity(self);

    self->velocity.y += .5;

    
}

void player_think(Entity* self)
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


    
    if ((keys[SDL_SCANCODE_SPACE] && (self->grounded)) && (self->jumpcool <= 0)) {

        self->frameAnimStart = 42;
        if (self->frame < 42) {
            self->frame = 42;
        }
        self->frameCount = 50;
        self->jumpcool = 15;
        self->velocity.y -= 15;
        //vector2d_scale(thrust, vector2d(0, 1), -15);
        //vector2d_add(self->velocity, self->velocity, thrust);

    }
    else if (keys[SDL_SCANCODE_D] && (self->attackcool == 0)) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 0;
        self->velocity.x = 1;
        //self->velocity.y += .58;
        //vector2d_scale(thrust, vector2d(1,0), 1.25);
        //vector2d_add(self->velocity, self->velocity, thrust);
        //vector2d_scale(thrust, vector2d(0, 1), 0.95);
        //vector2d_add(self->velocity, self->velocity, thrust);
        if (keys[SDL_SCANCODE_J]) {
            self->frameAnimStart = 77;
            if (self->frame < 77) {
                self->frame = 77;
            }
            self->attack = 3;
            self->attackcool = 15;
            self->frameCount = 84;
            self->velocity.x = 0;

        }
    }
    else if (keys[SDL_SCANCODE_A] && (self->attackcool == 0)) {
        self->frameAnimStart = 7;
        if (self->frame < 7) {
            self->frame = 7;
        }
        self->frameCount = 14;
        self->flip.x = 1;
        self->velocity.x = -1;
        //self->velocity.y += .58;
        //vector2d_scale(thrust, vector2d(1, 0), -1.25);
        //vector2d_add(self->velocity, self->velocity, thrust);
        //vector2d_scale(thrust, vector2d(0, 1), 0.95);
        //vector2d_add(self->velocity, self->velocity, thrust);
        if (keys[SDL_SCANCODE_J]) {
            self->frameAnimStart = 77;
            if (self->frame < 77) {
                self->frame = 77;
            }
            self->attack = 3;
            self->attackcool = 15;
            self->frameCount = 84;
            self->velocity.x = 0;

        }
    }
    else if (keys[SDL_SCANCODE_S] && (self->attackcool == 0)) {
        self->frameAnimStart = 65;
        if (self->frame < 65) {
            self->frame = 65;
        }
        self->frameCount = 67;
        self->velocity.x = 0;
        //vector2d_scale(thrust, vector2d(0, 1), 0.95);
        //vector2d_add(self->velocity, self->velocity, thrust);
        if (keys[SDL_SCANCODE_J]) {
            self->attack = 2;
            self->attackcool = 8;
            self->frameAnimStart = 86;
            if (self->frame < 86) {
                self->frame = 86;
            }
            self->frameCount = 90;
            if (self->flip.x == 1) {
                self->velocity.x = -0.75;

            }
            else {
                self->velocity.x = 0.75;

            }
        }
    }
    else if ((keys[SDL_SCANCODE_J]) && !(keys[SDL_SCANCODE_S]) && !(keys[SDL_SCANCODE_A]) && !(keys[SDL_SCANCODE_D]) && (self->attackcool == 0)) {
        self->frameAnimStart = 15;
        if (self->frame < 15) {
            self->frame = 15;
        }
        //i++;
        self->frameCount = 23;
        self->attackcool = 15;
        self->attack = 1;
        //player_melee(self);

        //vector2d_scale(thrust, vector2d(0, 1), 0.95);
        //vector2d_add(self->velocity, self->velocity, thrust);
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
    else if (self->attackcool > 0) {
        if (self->attack == 1) {
            if (self->frame >= self->frameCount - 0.5) {
                player_melee(self,self->str);
            }
            if ((keys[SDL_SCANCODE_J]) && (self->frame >= self->frameCount - 0.5)) {
                self->frameAnimStart = 24;
                if (self->frame < 24) {
                    self->frame = 24;
                }
                self->frameCount = 27;
                self->attackcool = 5;
                self->attack = 4;
            }
        }
        if (self->attack == 2) {
            if (self->frame >= self->frameCount - 0.05) {
                player_melee(self, self->str/2);
            }
            if (self->flip.x == 1) {
                self->velocity.x = -0.75;

            }
            else {
                self->velocity.x = 0.75;

            }
        }
        if (self->attack == 3) {
            if (self->frame >= self->frameCount - 0.05) {
                player_melee(self, self->str*2);
            }
        }
        if (self->attack == 4) {
            if (self->frame >= self->frameCount - 0.5) {
                player_melee(self, self->str);
            }
        }

    }
    else {
        self->frameAnimStart = 0;
        self->frameCount = 6;
        self->velocity.x = 0;
        //vector2d_scale(thrust, vector2d(0, 1), 0.95);
        //vector2d_add(self->velocity, self->velocity, thrust);
    }
    if ((keys[SDL_SCANCODE_K] && self->projectcool <= 0)) {
        Entity* dagger =  dagger_spawn(vector2d(self->position.x, self->position.y+6),self->flip);
        level_add_entity(dagger);
        //slog("shoot");
        self->projectcool = 15;
    }

}

int  player_damage(Entity* self, int amount, Entity* source)
{
    //slog("CRUNCH");
    self->health -= amount;

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
