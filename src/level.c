#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"
#include "camera.h"
#include "level.h"
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


static Level gamelevel = { 0 };

Level level_get()
{
    return gamelevel;
}

Level *level_new()
{
    Level *level;
    level = (Level *)malloc(sizeof(Level));
    if (!level)
    {
        slog("failed to allocate memory for the game level");
        return NULL;
    }
    memset(level,0,sizeof(Level));
    return level;
}

Level *level_load(const char *filename)
{
    const char *string;
    Level *level;
    SJson *json,*levelJS,*levelMap,*row;
    int rows,columns;
    int count,tileindex;
    int i,j;

    if (!filename)
    {
        slog("filename is NULL, cannot load the level");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)return NULL;

    level = level_new();
    if (!level)
    {
        sj_free(json);
        return NULL;
    }
    
    levelJS = sj_object_get_value(json,"level");
    if (!levelJS)
    {
        slog("level json missing level object");
        level_free(level);
        sj_free(json);
        return NULL;
    }
    
    string = sj_get_string_value(sj_object_get_value(levelJS,"bgImage"));
    if (string)
    {
        level->bgImage = gf2d_sprite_load_image((char *)string);
    }
    string = sj_get_string_value(sj_object_get_value(levelJS,"tileSet"));
    if (string)
    {
        sj_get_integer_value(sj_object_get_value(levelJS,"tileWidth"),&level->tileWidth);
        sj_get_integer_value(sj_object_get_value(levelJS,"tileHeight"),&level->tileHeight);
        sj_get_integer_value(sj_object_get_value(levelJS,"tileFPL"),&level->tileFPL);
        level->tileSet = gf2d_sprite_load_all(
            (char *)string,
            level->tileWidth,
            level->tileHeight,
            level->tileFPL);
    }

    levelMap = sj_object_get_value(levelJS,"tileMap");
    if (!levelMap)
    {
        slog("missing tileMap data");
        level_free(level);
        sj_free(json);
        return NULL;
    }
    rows = sj_array_get_count(levelMap);
    row = sj_array_get_nth(levelMap,0);
    columns = sj_array_get_count(row);
    count = rows * columns;
    level->levelWidth = columns;
    level->levelHeight = rows;
    level->tileMap = (TileTypes*)gfc_allocate_array(sizeof(TileTypes),count);
    if (!level->tileMap)
    {
        level_free(level);
        sj_free(json);
        return NULL;
    }
    level->tileCount = count;
    
    tileindex = 0;
    slog("tilemap data:");
    for (j = 0; j < rows; j++)
    {
        row = sj_array_get_nth(levelMap,j);
        if (!row)continue;// skip it, its bad
        if (columns != sj_array_get_count(row))
        {
            slog("row %i, column count mismatch",j);
            continue;
        }
        for (i = 0; i < columns; i++)
        {
            sj_get_integer_value(sj_array_get_nth(row,i),&level->tileMap[tileindex]);
            printf("%i,",level->tileMap[tileindex++]);
        }
        printf("\n");
    }
    slog("map width: %i",level->levelWidth);
    slog("map height: %i",level->levelHeight);
    
    gamelevel.space = gf2d_space_new_full(
        3,
        gf2d_rect(0, 0, 16000, 16000),
        0.1,
        vector2d(0, 0.1),
        1,
        1);

    Vector2D offset, drawPosition;
    offset = camera_get_offset();
    for (i = 0; i < level->tileCount; i++)
    {
        if (level->tileMap[i] == 0)continue;
        drawPosition.x = ((i % level->levelWidth) * level->tileSet->frame_w);
        drawPosition.y = ((i / level->levelWidth) * level->tileSet->frame_h);
        drawPosition.x += offset.x;
        drawPosition.y += offset.y;
        gf2d_space_add_static_shape(gamelevel.space, gf2d_shape_rect(drawPosition.x, drawPosition.y, level->tileSet->frame_w, level->tileSet->frame_h));
    }
    //gf2d_space_add_static_shape(gamelevel.space, gf2d_shape_edge(100, 200, 255, 360)); //for slope

    SJson* spawnList = sj_copy(sj_object_get_value(levelJS, "spawn"));

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
            slog("spawn Bomb");
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

    }

    sj_free(json);
    return level;
}

void level_free(Level *level)
{
    if (!level)return;// nothing to do
    
    if (level->tileMap != NULL)
    {
        free(level->tileSet);
        level->tileMap = NULL;
    }
    gf2d_sprite_free(level->bgImage);
    gf2d_sprite_free(level->tileSet);
    
    free(level);
}

void level_draw(Level *level)
{
    SDL_Rect camera;
    int i;
    Vector2D offset, drawPosition;
    if (!level)
    {
        slog("cannot draw level, NULL pointer provided");
        return;
    }
    // draw the background first
    gf2d_sprite_draw_image(level->bgImage,vector2d(0,0));
    //then draw the tiles
    
    if (!level->tileMap)
    {
        slog("not tiles loaded for the level, cannot draw it");
        return;
    }
    offset = camera_get_offset();
    for (i = 0; i < level->tileCount; i++)
    {
        if (level->tileMap[i] == 0)continue;
        drawPosition.x = ((i % level->levelWidth) * level->tileSet->frame_w);
        drawPosition.y = ((i / level->levelWidth) * level->tileSet->frame_h);
        if (!camera_rect_on_screen(gfc_sdl_rect(drawPosition.x, drawPosition.y, level->tileSet->frame_w, level->tileSet->frame_h)))
        {
            //tile is off camera, skip
            continue;
        }
        drawPosition.x += offset.x;
        drawPosition.y += offset.y;
        gf2d_sprite_draw(
            level->tileSet,
            drawPosition,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            level->tileMap[i] - 1);
        
        //if (gamelevel.space)gf2d_space_draw(gamelevel.space, vector2d(0,0));
    }
}

void level_update(Level* level)
{
    /*
    SDL_Rect camera;
    if (!level)return;
    camera = camera_get_rect();
    //snap camera to the level bounds
    if ((camera.x + camera.w) > (int)level->levelSize.x)
    {
        camera.x = level->levelSize.x - camera.w;
    }
    if ((camera.y + camera.h) > (int)level->levelSize.y)
    {
        camera.y = level->levelSize.y - camera.h;
    }
    if (camera.x < 0)camera.x = 0;
    if (camera.y < 0)camera.y = 0;
    camera_set_position(vector2d(camera.x, camera.y));
    */
    gf2d_entity_pre_sync_all();
    gf2d_space_update(gamelevel.space);
    gf2d_entity_post_sync_all();
    
    //gf2d_particle_emitter_update(gamelevel.pe);
}

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

void level_remove_entity(Entity* ent)
{
    if (!ent)return;
    if (!gamelevel.space)
    {//nothing to do
        return;
    }
    gf2d_space_remove_body(gamelevel.space, &ent->body);
}

void level_add_entity(Entity* ent)
{
    if (!ent)return;
    if (!gamelevel.space)
    {
        //slog("cannot add entity %s to level, no space defined!", ent->name);
        return;
    }
    if (ent->body.touch == NULL)
    {
        ent->body.touch = body_body_touch;
    }
    gf2d_space_add_body(gamelevel.space, &ent->body);
}

Space* level_get_space()
{
    return gamelevel.space;
}


/*file footer*/
