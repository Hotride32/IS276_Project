#ifndef __ROOM_H__
#define __ROOM_H__

#include "gfc_types.h"
#include "gfc_text.h"

#include "gf2d_sprite.h"
#include "gf2d_space.h"
//#include "level.h"
#include "entity.h"

typedef enum
{
    TT_Empty,
    TT_Brick,
    TT_
}TileTypes;

typedef struct
{
    Sprite     *bgImage;     /**<the background image for the room*/
    Sprite     *tileSet;     /**<sprite for the tileset*/
    Vector2D    roomSize;   /**<how large, in pixels, the room is*/
    TileTypes  *tileMap;     /**<the tiles for the room*/
    Uint32      tileCount;
    Uint32      roomWidth;  /**<how many tiles per row the room has*/
    Uint32      roomHeight; /**<how many tiles per column the room has*/
    int         tileWidth;   /**<now many pixels wide the tiles are*/
    int         tileHeight;  /**<how many pixels tall each tile is*/
    int         tileFPL;
    float       roomoffset;
    Space       *space;
    //room* room_list = {0};
   // Uint32      max_room;
    Bool       _inuse;
}Room;


/**
 * @brief allocate and initialize a room
 * @return NULL on error, else a pointer to a new room
 */
Room *room_new();

Room room_get();

void room_clear();

/**
 * @brief load a room from a json file
 * @param filename the file to load
 * @return NULL on error (See the logs) a loaded room otherwise
 */
Room *room_load(const char *filename);

Room* room_loadRoom(const char* filename, float offsetForRoom);

/**
 * @brief free up a previously loaded room
 * @param room a pointer to the room to free
 */
void room_free(Room *room);


/**
 * @brief draw a room
 * @param room the room the draw
 */
void room_draw(Room *room);

void room_update(Room* room);
/*
void room_add_entity(Entity* ent);

void room_remove_entity(Entity* ent);

Space* room_get_space();
*/

#endif
