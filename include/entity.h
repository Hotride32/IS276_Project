#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "simple_json.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_collision.h"

typedef struct Entity_s
{
    Bool       _inuse;
    Vector2D    position;
    Vector2D    spawnPos;
    Vector2D    velocity;
    Vector2D    flip;
    Vector2D    facing;
    Vector3D    rotation; //(x,y) = rotation center, z = degrees of rotation
    Sprite     *sprite; 
    Sprite*     laser;
    Shape       shape;
    Body        body;
    char        hitLayer;
    float       frame;
    float       frameRate;
    int         frameCount;
    int         frameAnimStart;
    int         dashcool;
    void      (*activate)(struct Entity_S* self, struct Entity_S* activator);
    void      (*update)(struct Entity_s *self);
    void      (*think)(struct Entity_s *self);
    void      (*draw)(struct Entity_s *self);
    void      (*free)(struct Entity_s *self);
    int  (*touch)(struct Entity_S* self, struct Entity_S* other);/**<when this entity touches another entity*/
    int  (*damage)(struct Entity_S* self, int amount, struct Entity_S* source);/**<when this entity takes damage*/
    void (*die)(struct Entity_S* self);     /**<when this entity dies*/

    float health;                           /**<health of entity*/
    float maxHealth;                        /**<maximum health of entity*/
    float magicPt;
    float  maxMagicPt;
    int   mag;
    int   str;
    int   def;
    float   bombCount;
    float   knifeCount;
    float   axeCount;
    float   maxbombCount;
    float   maxknifeCount;
    float   maxaxeCount;
    int   cooldown;                         /**<useful for timing cooldowns*/
    int   attack;                           /**<which attack is being performed*/
    int   count;                            /**<useful for counting things like ammo count or health ammount*/
    float attackcool;
    float jumpcool;
    float projectcool;
    int   jumpcount;                        /**<used for multijump*/
    int   grounded;
    void       *data;
}Entity;

/**
 * @brief initialize internal entity management system
 * @param max_entities how many concurrent entities to support
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief calls update function on all entities
 */
void entity_manager_update_entities();


/**
 * @brief call think function for all entities
 */
void entity_manager_think_entities();

/**
 * @brief call draw on all entities
 */
void entity_manager_draw_entities();


/**
 * @brief free all entities in the system and destroy entity manager
 */
void entity_manager_free();

/**
 * @brief allocate an entity, initialize it to zero and return a pointer to it
 * @return NULL on error (see logs) or a pointer to an initialized entity.
 */
Entity *entity_new();

/**
 * @brief frees provided entity
 * @param ent the entity to free
 */
void entity_free(Entity *ent);

/**
 * @brief draws an entity to the current render frame
 * @param ent the entity to draw
 */
void entity_draw(Entity *ent);

/**
 * @brief adjust position of entity to redice clipping static shapes of the world
 * @param self the entity to adjust
 */
void entity_world_snap(Entity* self);

/**
 * @brief adjust velcity according to gravity/ test for grounded status
 * @param self the entity to adjust
 */
void entity_apply_gravity(Entity* self);

/**
 * @brief check if the entity is about to walk off the edge of a platform
 * @note useful for AI
 * @param self the entity to check for
 * @return 0 if there is no more platform, 1 if there is something there
 *
 */
int entity_platform_end_check(Entity* self);

/**
 * @brief check if the entity is adjacent to a static shape in the world.
 * @note this is used for testing if the entity is on the ground or to prevent getting stuck on a wall
 * @param self the entity to check for
 * @param dir the direction to offset by
 * @returns 0 if all clear, 1 if the world can be touched
 */
int entity_wall_check(Entity* self, Vector2D dir);

/**
 * @brief push other entity away from self by amount
 */
void entity_push(Entity* self, Entity* other, float amount);

List* entity_get_clipped_entities(Entity* self, Shape s, Uint32 layers, Uint32 team);

void gf2d_entity_pre_sync_all();

void gf2d_entity_post_sync_all();

void entity_damage(Entity* target, Entity* killer, int damage, float kick);

Entity* entity_get_touching_player(Entity* self);

#endif
