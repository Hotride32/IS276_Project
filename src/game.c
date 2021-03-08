#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "gf2d_space.h"
#include "gf2d_collision.h"


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Level *level;
    
    int mx,my;
    float mf = 0;
    
    Space* space = NULL;
    Collision collision;
    CollisionFilter filter = { 0 };
    static Body body[10000];// not a pointer!
    Shape shape[4];// not a pointer!    /*parse args*/
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    camera_set_dimensions(vector2d(1200, 720));
    camera_set_position(vector2d(0, 0));
    gf2d_sprite_init(1024);
    entity_manager_init(100);

    
    SDL_ShowCursor(SDL_DISABLE);

    /*space = gf2d_space_new_full(
        3,
        gf2d_rect(0,0,1200,700),
        0.1,
        vector2d(0,0.1),
        1,
        1);*/
    
    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    level = level_load("levels/exampleLevel.json");
    Entity* player = player_spawn(vector2d(100,435));
    Entity* ramp = ramp_spawn(vector2d(224, 450));
    level_add_entity(player);
    //level_add_entity(ramp);

    /*

    shape[1] = gf2d_shape_circle(0, 0, 10);
    shape[2] = gf2d_shape_circle(10, 0, 15);
    shape[3] = gf2d_shape_rect(-32, -32, 64, 64);
    shape[0] = gf2d_shape_rect(-16, -16, 32, 32);

    gf2d_space_add_static_shape(space, gf2d_shape_rect(200, 500, 512, 32));
    gf2d_space_add_static_shape(space, gf2d_shape_rect(610, 50, 30, 550));
    gf2d_space_add_static_shape(space, gf2d_shape_circle(300, 300, 15));
    gf2d_space_add_static_shape(space, gf2d_shape_edge(100, 200, 255, 360));
    gf2d_space_add_static_shape(space, gf2d_shape_edge(100, 400, 255, 360));
    gf2d_space_add_static_shape(space, gf2d_shape_edge(100, 200, 100, 400));
    
    gf2d_body_set(
        &body[0],
        "body",
        1,
        0,
        0,
        0,
        vector2d(256, 256),
        vector2d(2.3, 4.4),
        10,
        1,
        1,  //elasticity
        &shape[0],
        NULL,
        NULL);
    gf2d_space_add_body(space, &body[0]);

    gf2d_space_add_body(space, &player->body);
    */
    /*main game loop*/
    filter.worldclip = 1;


    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        entity_manager_think_entities();
        entity_manager_update_entities();
        
        //camera_set_position(player->position);

        level_update(level);
        


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            level_draw(level);
            
            
            
            //gf2d_space_draw(space, vector2d(0, 0));
            /*if (collision.collided)
            {
                gf2d_draw_line(vector2d(mx, my), collision.pointOfContact, vector4d(255, 0, 0, 255));
            }
            else
            {
                gf2d_draw_line(vector2d(mx, my), vector2d(600, 360), vector4d(255, 255, 0, 255));
            }*/

            entity_manager_draw_entities();

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        /*
        if (keys[SDL_SCANCODE_D]) {
            camera_move(vector2d(1, 0));
        }
        if (keys[SDL_SCANCODE_A]) {
            camera_move(vector2d(-1, 0));
        }
        if (keys[SDL_SCANCODE_UP]) {
            camera_move(vector2d(0, -10));
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            camera_move(vector2d(0, 10));
        }
        */
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
//        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
