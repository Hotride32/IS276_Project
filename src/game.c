#include <SDL.h>
#include <stdio.h>
#include <math.h>

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_audio.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
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
#include "tile.h"
#include "level.h"
#include "gf2d_space.h"
#include "gf2d_collision.h"
#include "gf2d_font.h"
#include "gf2d_mouse.h"
#include "gf2d_draw.h"
#include "gf2d_actor.h"
#include "windows_common.h"

static int _done = 0;
static int changer = 1;
static Window* _quit = NULL;
static Window* edit = NULL;
static Window* mainWin = NULL;
static Level* level = NULL;
//static Level* level2 = NULL;
static Entity* player = NULL;

void onCancel(void* data)
{
    _quit = NULL;
}
void onCancel2(void* data)
{
    _done = 1;
    mainWin = NULL;
}
void onOK(void* data)
{
    mainWin = NULL;
    changer = 0;
    level = level_load("levels/Level1.json",0);
    //level2 = level_loadRoom("levels/Level1.json",768.0);
    player = player_spawn(vector2d(100, 439), "levels/player.json");
}
void onOK2(void* data) {
    
    level = NULL;
    player = NULL;
    gf2d_entity_free_all();
    camera_set_position(vector2d(0, 0));
    level = level_load("levels/Level1.json",1);
    edit = NULL;
    changer = 2;
    //player = player_spawn(vector2d(100, 435), "levels/player.json");

}
void onCancel3(void* data)
{
    //edit = NULL;
    changer = 1;
    mainWin = window_yes_no_level("Choose Level", onOK, onCancel2, NULL, NULL);
}
void onExit(void* data)
{
    //_done = 1;
    //level_free(level);
    //level_clear();
    if (player && changer == 0) {
        player_save(player, "levels/player.json");
    }
    level = NULL;
    //level_clear();
    //level_free(level);
    //level2 = NULL;
    mainWin = window_yes_no_level("Choose Level", onOK, onCancel2, NULL, NULL);
    gf2d_entity_free_all();
    camera_set_position(vector2d(0, 0));
    _quit = NULL;
    changer = 1;

}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    //int done = 0;
    const Uint8 * keys;
    //Level *level;
    Font* font;
    TextLine player_text;
    TextLine MP_text;
    int i;
    int fullscreen = 0;
    int debug = 0;
    Sprite* background = NULL;
    

    int mx,my;
    float mf = 0;
    float cool = 0;
    
    
    Space* space = NULL;
    Collision collision;
    CollisionFilter filter = { 0 };
    static Body body[10000];// not a pointer!
    Shape shape[4];// not a pointer!    /*parse args*/
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--fullscreen") == 0)
        {
            fullscreen = 1;
        }
        if (strcmp(argv[i], "--debug") == 0)
        {
            debug = 1;
        }
    }

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0, 0, 0, 255),
        fullscreen,
        debug);
    gf2d_graphics_set_frame_delay(16);
    gfc_audio_init(256, 16, 4, 1, 1, 1);
    //camera_set_dimensions(vector2d(1200, 720));
    //camera_set_dimensions(vector2d( 480, 288));
    camera_set_position(vector2d(0, 0));
    gf2d_sprite_init(1024);
    gf2d_action_list_init(128);
    gf2d_font_init("config/font.cfg");
    gfc_input_init("config/input.cfg");
    gf2d_windows_init(128);
    entity_manager_init(500);
    //font_init(10);

    camera_set_dimensions(vector2d(1200, 700));
    background = gf2d_sprite_load_image("images/backgrounds/background_dark.png");
    Sound* bgm = gfc_sound_load("music/bensound-betterdays.mp3", 1.0, 1);
    gfc_sound_play(bgm, 10000, 1.0, -1, -1);

    SDL_ShowCursor(SDL_DISABLE);

    gf2d_mouse_load("actors/mouse.actor");
    mf = 0;
    /*space = gf2d_space_new_full(
        3,
        gf2d_rect(0,0,1200,700),
        0.1,
        vector2d(0,0.1),
        1,
        1);*/
    
    /*demo setup*/
    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

    //level = level_load("levels/exampleLevel.json");

    /*
    ramp_spawn(vector2d(224, 450));
    breakable_spawn(vector2d(600, 444));
    spike_spawn(vector2d(500, 450));
    checkpoint_spawn(vector2d(450, 444));
    checkpoint_spawn(vector2d(150, 444));*/
    //Entity* monster = monster_spawn(vector2d(700, 444));
    //Entity* player = NULL;
    //player = player_spawn(vector2d(100, 435), "levels/player.json");


    /*monster_spawn(vector2d(700, 435));
    skull_spawn(vector2d(400, 300));
    skeleton_spawn(vector2d(800, 400));
    tower_spawn(vector2d(900, 400));
    angel_spawn(vector2d(1000, 350));
    health_spawn(vector2d(100, 260));
    magic_spawn(vector2d(120, 260));
    axePick_spawn(vector2d(140, 260));
    knifePick_spawn(vector2d(160, 260));
    //bombPick_spawn(vector2d(180, 260));

    magicBoss_spawn(vector2d(1750, 350));
    al_spawn(vector2d(450, 100));
    */
    if (changer == 1) {
        mainWin = window_yes_no_level("Choose Level", onOK, onCancel2, NULL, NULL);
    }
    //level_add_entity(player);
    //level_add_entity(ramp);
    
    //font = font_load("fonts/Xenogears_font.ttf", 16);
    
    
    //font = font_load("fonts/colony_wars.ttf", 8);
    
    /*main game loop*/
    filter.worldclip = 1;


    /*main game loop*/
    while (!_done)
    {
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        if (cool > 0) {
            cool -= 1;
        }

        gf2d_windows_update_all();

        gf2d_mouse_update();
        if (changer == 0) {
            entity_manager_think_entities();
            entity_manager_update_entities();
        }
        //camera_set_position(player->position);
        if (changer != 1) {
            level_update(level);
        }

        //entity_manager_update_entities();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        gf2d_sprite_draw_image(background, vector2d(0, 0));
        if (changer != 1) {
            level_draw(level);
            //level_draw(level2);
        }
        //entity_manager_update_entities();

        if (changer != 1) {
            entity_manager_draw_entities();
        }
        //UI elements last

        //gf2d_draw_rect(gf2d_rect_to_sdl_rect(gf2d_rect(0, 0, 1200, 70)), gfc_color8(0, 0, 0, 255));
        /*
        gf2d_sprite_draw(
            mouse,
            vector2d(mx, my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);
            */
        /*
        gfc_line_sprintf(player_text, "Health : %0.0f/%0.0f ", player->health, player->maxHealth);
        font_render(font, player_text, vector2d(6, 6), gfc_color8(255, 0, 0, 255));

        gfc_line_sprintf(player_text, "MP : %0.0f/%0.0f ", player->magicPt, player->maxMagicPt);
        font_render(font, player_text, vector2d(6, 18), gfc_color8(0, 0, 255, 255));

        gfc_line_sprintf(player_text, "Knife : %0.0f ", player->knifeCount);
        font_render(font, player_text, vector2d(100, 6), gfc_color8(255, 255, 255, 255));

        gfc_line_sprintf(player_text, "Axe : %0.0f ", player->axeCount);
        font_render(font, player_text, vector2d(200, 6), gfc_color8(255, 255, 255, 255));

        gfc_line_sprintf(player_text, "Bomb : %0.0f ", player->bombCount);
        font_render(font, player_text, vector2d(300, 6), gfc_color8(255, 255, 255, 255));
        */

        if (changer == 0 && player != NULL) {

            player = player_get();

            gfc_line_sprintf(player_text, "Health : %0.0f/%0.0f ", player->health, player->maxHealth);
            gf2d_font_draw_line_tag(player_text, FT_H1, gfc_color8(255, 0, 0, 255), vector2d(32, 32));

            gfc_line_sprintf(player_text, "MP : %0.0f/%0.0f ", player->magicPt, player->maxMagicPt);
            // font_render(font, player_text, vector2d(32, 64), gfc_color8(0, 0, 255, 255));
            gf2d_font_draw_line_tag(player_text, FT_H1, gfc_color8(0, 0, 255, 255), vector2d(32, 64));

            gfc_line_sprintf(player_text, "Knife : %0.0f ", player->knifeCount);
            // font_render(font, player_text, vector2d(400, 32), gfc_color8(255, 255, 255, 255));
            gf2d_font_draw_line_tag(player_text, FT_H1, gfc_color8(255, 255, 255, 255), vector2d(400, 32));

            gfc_line_sprintf(player_text, "Axe : %0.0f ", player->axeCount);
            // font_render(font, player_text, vector2d(600, 32), gfc_color8(255, 255, 255, 255));
            gf2d_font_draw_line_tag(player_text, FT_H1, gfc_color8(255, 255, 255, 255), vector2d(600, 32));

            gfc_line_sprintf(player_text, "Bomb : %0.0f ", player->bombCount);
            //font_render(font, player_text, vector2d(800, 32), gfc_color8(255, 255, 255, 255));
            gf2d_font_draw_line_tag(player_text, FT_H1, gfc_color8(255, 255, 255, 255), vector2d(800, 32));
            
            if (keys[SDL_SCANCODE_V] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                tile_spawn(mouse);
                cool = 60;
            }

        }

        if (changer == 2) {

            if (keys[SDL_SCANCODE_V] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                monster_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_B] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                skull_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_N] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                skeleton_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_H] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                tower_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_J] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                angel_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_K] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                magicBoss_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_L] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                al_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_Y] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                ramp_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_U] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                spike_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_I] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                breakable_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_O] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                checkpoint_spawn(mouse);
                cool = 60;
            }
            if (keys[SDL_SCANCODE_P] && cool == 0) {
                Vector2D offset = camera_get_offset();
                Vector2D mouse = gf2d_mouse_get_position();
                mouse.x -= offset.x;
                mouse.y -= offset.y;
                tile_spawn(mouse);
                cool = 60;
            }

            if (keys[SDL_SCANCODE_S]) {
                gf2d_entity_save_all("levels/editor/spawnlist.json");
                cool = 60;
            }
        }

        gf2d_windows_draw_all();
        gf2d_mouse_draw();
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        


        if ((gfc_input_command_down("exit")) && (_quit == NULL) && (mainWin == NULL) && (edit == NULL))
        {
            _quit = window_yes_no("Exit?", onExit, onCancel, NULL, NULL);
            /*if (player) {
                player_save(player, "levels/player.json");
            }*/
        }

        if((keys[SDL_SCANCODE_E]) && (_quit == NULL) && (edit == NULL) && (mainWin == NULL) && (changer != 2))
        {
            //mainWin = NULL;
            edit = window_yes_no("Open Editor?", onOK2, onCancel3, NULL, NULL);
          
        }

        /*
        if (keys[SDL_SCANCODE_ESCAPE]) {
            player_save(player, "levels/player.json");
        _done = 1; // exit condition

        }
        */
//        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
