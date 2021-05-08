#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"
#include "camera.h"
#include "room.h"
#include "entity.h"
#include "ramp.h"
#include "skull.h"
#include "skeleton.h"
#include "tower.h"
#include "magicboss.h"
#include "al.h"
#include "angel.h"
#include "pickup.h"
#include "breakable.h"
#include "checkpoint.h"
#include "monster.h"
#include "spike.h"
#include "companion.h"
#include "tile.h"
#include "level.h"


static Room gameroom = { 0 };

Room room_get()
{
    return gameroom;
}

void room_clear()
{
    gf2d_space_free(gameroom.space);
    gf2d_sprite_free(gameroom.bgImage);
    gf2d_sprite_free(gameroom.tileSet);
    //gf2d_sprite_free(gameroom.tileLayer);
    //gf2d_particle_emitter_free(gameroom.pe);
    /*if (gameroom.backgroundMusic)
    {
        Mix_FreeMusic(gameroom.backgroundMusic);
    }*/
    memset(&gameroom, 0, sizeof(Room));
    slog("room cleared");
}

Room *room_new()
{
    Room *room;
    room = (Room *)malloc(sizeof(Room));
    if (!room)
    {
        slog("failed to allocate memory for the game room");
        return NULL;
    }
    memset(room,0,sizeof(room));
    return room;
}

Room *room_load(const char *filename)
{
    const char *string;
    Room *room;
    SJson *json,*roomJS,*roomMap,*row;
    int rows,columns;
    int count,tileindex;
    int i,j;

    if (!filename)
    {
        slog("filename is NULL, cannot load the room");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)return NULL;

    room = room_new();
    if (!room)
    {
        sj_free(json);
        return NULL;
    }
    
    roomJS = sj_object_get_value(json,"level");
    if (!roomJS)
    {
        slog("room json missing room object");
        room_free(room);
        sj_free(json);
        return NULL;
    }
    
    string = sj_get_string_value(sj_object_get_value(roomJS,"bgImage"));
    if (string)
    {
        room->bgImage = gf2d_sprite_load_image((char *)string);
    }
    string = sj_get_string_value(sj_object_get_value(roomJS,"tileSet"));
    if (string)
    {
        sj_get_integer_value(sj_object_get_value(roomJS,"tileWidth"),&room->tileWidth);
        sj_get_integer_value(sj_object_get_value(roomJS,"tileHeight"),&room->tileHeight);
        sj_get_integer_value(sj_object_get_value(roomJS,"tileFPL"),&room->tileFPL);
        room->tileSet = gf2d_sprite_load_all(
            (char *)string,
            room->tileWidth,
            room->tileHeight,
            room->tileFPL);
    }

    roomMap = sj_object_get_value(roomJS,"tileMap");
    if (!roomMap)
    {
        slog("missing tileMap data");
        room_free(room);
        sj_free(json);
        return NULL;
    }
    rows = sj_array_get_count(roomMap);
    row = sj_array_get_nth(roomMap,0);
    columns = sj_array_get_count(row);
    count = rows * columns;
    room->roomWidth = columns;
    room->roomHeight = rows;
    room->tileMap = (TileTypes*)gfc_allocate_array(sizeof(TileTypes),count);
    if (!room->tileMap)
    {
        room_free(room);
        sj_free(json);
        return NULL;
    }
    room->tileCount = count;
    
    tileindex = 0;
    slog("tilemap data:");
    for (j = 0; j < rows; j++)
    {
        row = sj_array_get_nth(roomMap,j);
        if (!row)continue;// skip it, its bad
        if (columns != sj_array_get_count(row))
        {
            slog("row %i, column count mismatch",j);
            continue;
        }
        for (i = 0; i < columns; i++)
        {
            sj_get_integer_value(sj_array_get_nth(row,i),&room->tileMap[tileindex]);
            printf("%i,",room->tileMap[tileindex++]);
        }
        printf("\n");
    }
    slog("map width: %i",room->roomWidth);
    slog("map height: %i",room->roomHeight);
    
    gameroom.space = gf2d_space_new_full(
        3,
        gf2d_rect(0, 0, 160000, 160000),
        0.1,
        vector2d(0, 0.1),
        1,
        1);

    Vector2D offset, drawPosition;
    offset = camera_get_offset();
    for (i = 0; i < room->tileCount; i++)
    {
        if (room->tileMap[i] == 0)continue;
        drawPosition.x = ((i % room->roomWidth) * room->tileSet->frame_w);
        drawPosition.y = ((i / room->roomWidth) * room->tileSet->frame_h);
        drawPosition.x += offset.x;
        drawPosition.y += offset.y;
        gf2d_space_add_static_shape(level_get_space(), gf2d_shape_rect(drawPosition.x, drawPosition.y, room->tileSet->frame_w, room->tileSet->frame_h));
    }
    //gf2d_space_add_static_shape(gameroom.space, gf2d_shape_edge(100, 200, 255, 360)); //for slope

    SJson* spawnList = sj_copy(sj_object_get_value(roomJS, "spawn"));

    int r = 0, coun = 0, ent = 0;
    SJson *item;
    float positionX, positionY;
    int id = 0;
    coun  = sj_array_get_count(spawnList);
    slog("moncount: %i ", coun);
    
    for (r = 0; r < coun; r++)
    {
        //         ent += 1.0f;
        //          slog("moncount: %f ", ent);
        item = sj_array_get_nth(spawnList, r);
        if (!item)continue;
        sj_get_float_value(sj_object_get_value(item, "position_x"), &positionX);
        sj_get_float_value(sj_object_get_value(item, "position_y"), &positionY);
        if (!sj_get_integer_value(sj_object_get_value(item, "name"), &id))
        {
            id = 0;
        }
        if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")),"bomb") == 0) {
            //slog("spawn Bomb");
            bombPick_spawn(vector2d(positionX,positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "axe") == 0) {
            
            axePick_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "knife") == 0) {

            knifePick_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "magic") == 0) {

            magic_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "health") == 0) {

            health_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "angel") == 0) {

            angel_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "skull") == 0) {

            skull_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "skeleton") == 0) {

            skeleton_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "slime") == 0) {

            monster_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "tower") == 0) {

            tower_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "al") == 0) {

            al_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "wiz") == 0) {

            magicBoss_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "spike") == 0) {

            spike_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "checkpoint") == 0) {

            checkpoint_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "breakable") == 0) {

            breakable_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "ramp") == 0) {

            ramp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "boot") == 0) {

            boots_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "shield") == 0) {

            shield_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "sword") == 0) {

            sword_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "HP") == 0) {

            hp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "MP") == 0) {

            mp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "hpfairy") == 0) {

            companion_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "mpfairy") == 0) {

            mpFairy_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "daggerfairy") == 0) {

            daggerFairy_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "tile") == 0) {

            tile_spawn(vector2d(positionX, positionY));
        }

    }
    
   // room_loadRoom(filename, 768);

    sj_free(json);
    return room;
}

Room* room_loadRoom(const char* filename, float offsetForRoom)
{
    const char* string;
    Room* room;
    SJson* json, * roomJS, * roomMap, * row;
    int rows, columns;
    int count, tileindex;
    int i, j;

    if (!filename)
    {
        slog("filename is NULL, cannot load the room");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)return NULL;

    room = room_new();
    if (!room)
    {
        sj_free(json);
        return NULL;
    }

    roomJS = sj_object_get_value(json, "level");
    if (!roomJS)
    {
        slog("room json missing room object");
        room_free(room);
        sj_free(json);
        return NULL;
    }

    
    string = sj_get_string_value(sj_object_get_value(roomJS, "tileSet"));
    if (string)
    {
        sj_get_integer_value(sj_object_get_value(roomJS, "tileWidth"), &room->tileWidth);
        sj_get_integer_value(sj_object_get_value(roomJS, "tileHeight"), &room->tileHeight);
        sj_get_integer_value(sj_object_get_value(roomJS, "tileFPL"), &room->tileFPL);
        room->tileSet = gf2d_sprite_load_all(
            (char*)string,
            room->tileWidth,
            room->tileHeight,
            room->tileFPL);
    }

    roomMap = sj_object_get_value(roomJS, "tileMap");
    if (!roomMap)
    {
        slog("missing tileMap data");
        room_free(room);
        sj_free(json);
        return NULL;
    }
    rows = sj_array_get_count(roomMap);
    row = sj_array_get_nth(roomMap, 0);
    columns = sj_array_get_count(row);
    count = rows * columns;
    room->roomWidth = columns;
    room->roomHeight = rows;
    room->tileMap = (TileTypes*)gfc_allocate_array(sizeof(TileTypes), count);
    if (!room->tileMap)
    {
        room_free(room);
        sj_free(json);
        return NULL;
    }
    room->tileCount = count;

    tileindex = 0;
    slog("tilemap data:");
    for (j = 0; j < rows; j++)
    {
        row = sj_array_get_nth(roomMap, j);
        if (!row)continue;// skip it, its bad
        if (columns != sj_array_get_count(row))
        {
            slog("row %i, column count mismatch", j);
            continue;
        }
        for (i = 0; i < columns; i++)
        {
            sj_get_integer_value(sj_array_get_nth(row, i), &room->tileMap[tileindex]);
            printf("%i,", room->tileMap[tileindex++]);
        }
        printf("\n");
    }
    slog("map width: %i", room->roomWidth);
    slog("map height: %i", room->roomHeight);

    room->roomoffset = offsetForRoom;

    Vector2D offset, drawPosition;
    offset = camera_get_offset();
    for (i = 0; i < room->tileCount; i++)
    {
        if (room->tileMap[i] == 0)continue;
        drawPosition.x = ((i % room->roomWidth) * room->tileSet->frame_w);
        drawPosition.y = ((i / room->roomWidth) * room->tileSet->frame_h);
        drawPosition.x += offset.x + room->roomoffset;
        drawPosition.y += offset.y;
        gf2d_space_add_static_shape(level_get_space(), gf2d_shape_rect(drawPosition.x, drawPosition.y, room->tileSet->frame_w, room->tileSet->frame_h));
    }
    //gf2d_space_add_static_shape(gameroom.space, gf2d_shape_edge(100, 200, 255, 360)); //for slope

    SJson* spawnList = sj_copy(sj_object_get_value(roomJS, "spawn"));

    int r = 0, coun = 0, ent = 0;
    SJson* item;
    float positionX, positionY;
    int id = 0;
    coun = sj_array_get_count(spawnList);
    slog("moncount: %i ", coun);

    for (r = 0; r < coun; r++)
    {
        //         ent += 1.0f;
        //          slog("moncount: %f ", ent);
        item = sj_array_get_nth(spawnList, r);
        if (!item)continue;
        sj_get_float_value(sj_object_get_value(item, "position_x"), &positionX);
        sj_get_float_value(sj_object_get_value(item, "position_y"), &positionY);
        if (!sj_get_integer_value(sj_object_get_value(item, "name"), &id))
        {
            id = 0;
        }
        positionX += offsetForRoom;
        if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "bomb") == 0) {
            //slog("spawn Bomb");
            bombPick_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "axe") == 0) {

            axePick_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "knife") == 0) {

            knifePick_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "magic") == 0) {

            magic_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "health") == 0) {

            health_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "angel") == 0) {

            angel_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "skull") == 0) {

            skull_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "skeleton") == 0) {

            skeleton_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "slime") == 0) {

            monster_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "tower") == 0) {

            tower_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "al") == 0) {

            al_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "wiz") == 0) {

            magicBoss_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "spike") == 0) {

            spike_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "checkpoint") == 0) {

            checkpoint_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "breakable") == 0) {

            breakable_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "ramp") == 0) {

            ramp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "boot") == 0) {

            boots_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "shield") == 0) {

            shield_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "sword") == 0) {

            sword_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "HP") == 0) {

            hp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "MP") == 0) {

            mp_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "hpfairy") == 0) {

            companion_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "mpfairy") == 0) {

            mpFairy_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "daggerfairy") == 0) {

            daggerFairy_spawn(vector2d(positionX, positionY));
        }
        else if (strcmp(sj_get_string_value(sj_object_get_value(item, "name")), "tile") == 0) {

            tile_spawn(vector2d(positionX, positionY));
        }

    }

    sj_free(json);
    return room;
}


void room_free(Room *room)
{
    if (!room)return;// nothing to do
    
    if (room->tileMap != NULL)
    {
        free(room->tileSet);
        room->tileMap = NULL;
    }
    gf2d_sprite_free(room->bgImage);
    gf2d_sprite_free(room->tileSet);
    
    free(room);
}

void room_draw(Room *room)
{
    SDL_Rect camera;
    int i;
    Vector2D offset, drawPosition;
    if (!room)
    {
        slog("cannot draw room, NULL pointer provided");
        return;
    }
    // draw the background first
    if (room->bgImage) {
        gf2d_sprite_draw_image(room->bgImage, vector2d(0, 0));
    }
    //then draw the tiles
    
    if (!room->tileMap)
    {
        slog("not tiles loaded for the room, cannot draw it");
        return;
    }
    offset = camera_get_offset();
    for (i = 0; i < room->tileCount; i++)
    {
        if (room->tileMap[i] == 0)continue;
        drawPosition.x = ((i % room->roomWidth) * room->tileSet->frame_w);
        drawPosition.y = ((i / room->roomWidth) * room->tileSet->frame_h);
        if (room->roomoffset) {
            if (!camera_rect_on_screen(gfc_sdl_rect(drawPosition.x + room->roomoffset, drawPosition.y, room->tileSet->frame_w, room->tileSet->frame_h)))
            {
                //tile is off camera, skip
                continue;
            }
            drawPosition.x += offset.x + room->roomoffset;
            drawPosition.y += offset.y;
        }
        else {
            if (!camera_rect_on_screen(gfc_sdl_rect(drawPosition.x, drawPosition.y, room->tileSet->frame_w, room->tileSet->frame_h)))
            {
                //tile is off camera, skip
                continue;
            }
            drawPosition.x += offset.x;
            drawPosition.y += offset.y;
        }
        gf2d_sprite_draw(
            room->tileSet,
            drawPosition,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            room->tileMap[i] - 1);
        
        //if (gameroom.space)gf2d_space_draw(gameroom.space, vector2d(0,0));
    }
    
}

void room_update(Room* room)
{
    /*
    SDL_Rect camera;
    if (!room)return;
    camera = camera_get_rect();
    //snap camera to the room bounds
    if ((camera.x + camera.w) > (int)room->roomSize.x)
    {
        camera.x = room->roomSize.x - camera.w;
    }
    if ((camera.y + camera.h) > (int)room->roomSize.y)
    {
        camera.y = room->roomSize.y - camera.h;
    }
    if (camera.x < 0)camera.x = 0;
    if (camera.y < 0)camera.y = 0;
    camera_set_position(vector2d(camera.x, camera.y));
    */
    //gf2d_entity_pre_sync_all();
    //gf2d_space_update(gameroom.space);
    //gf2d_entity_post_sync_all();
    
    //gf2d_particle_emitter_update(gameroom.pe);
}


/*
int body_body_touch(Body* self, List* collisionList)
{
    Entity* selfEnt;
    Collision* c;
    int i, count;
    if (!self)return 0;
    selfEnt = (Entity*)self->data;
    if (!selfEnt->touch)return 0;
    count = gfc_list_get_count(collisionList);
    for (i = 0; i < count; i++)
    {
        c = (Collision*)gfc_list_get_nth(collisionList, i);
        if (!c)continue;
        if (!c->body)continue;
        if (!c->body->data)continue;
        selfEnt->touch(selfEnt, (Entity*)c->body->data);
    }
    return 0;
}


void room_remove_entity(Entity* ent)
{
    if (!ent)return;
    if (!gameroom.space)
    {//nothing to do
        return;
    }
    gf2d_space_remove_body(gameroom.space, &ent->body);
}

void room_add_entity(Entity* ent)
{
    if (!ent)return;
    if (!gameroom.space)
    {
        //slog("cannot add entity %s to room, no space defined!", ent->name);
        return;
    }
    if (ent->body.touch == NULL)
    {
        ent->body.touch = body_body_touch;
    }
    gf2d_space_add_body(gameroom.space, &ent->body);
}

Space* room_get_space()
{
    return gameroom.space;
}
*/

/*file footer*/
