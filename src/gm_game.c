#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <system.h>
#include <gm_game.h>
#include <input.h>
#include <actors.h>
#include <background.h>
#include <room.h>
#include <transition.h>


void game_init()
{
    SPR_init();
    actors_clear_all();
    BG_load_frame();
    BG_display_frame();
    VDP_loadTileSet(&castle_tileset,BG_VRAM_ind,DMA);
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    PAL_setPalette(PAL2, castle_palette.data, DMA);
    actors_init();
    gm_state=GAME_STATE_NORMAL;
    gm_timer=0;
    func_update=game_update;
    room_init();
    //XGM_startPlay(bgm_fjf);
}

void game_end()
{
    SPR_end();
}

void game_run_move()
{
    game_pixels_scrolled+=2;
    game_move_actors();
    if (game_pixels_scrolled>=16)
    {
        game_pixels_scrolled=0;
        gm_state=GAME_STATE_NORMAL;
        game_move_end();
        game_check_turn();
    }

}

void game_draw_hud_text()
{

    VDP_drawTextBG(BG_B,"Castle",28,2);
    VDP_drawTextBG(BG_B,"  Kranion",28,3);
    VDP_drawTextBG(BG_B,"Floor 01",28,5);

    VDP_drawTextBG(BG_B,"Swordsman",28,7);
}

void game_run_gate()
{
    if (gm_timer==0)
    {
        gate_speed=0;
        SPR_setPriority(player->sprite,true);
        VDP_setHilightShadow(true);
    }
    
    gm_timer++;
    player->scroll_y-=gate_speed;
    player->timer++;
    if (player->timer == 4)
    {   
        gate_speed+=1;
        player->timer = 0;
        player->facing_dir+=1;
        if (player->facing_dir > DIR_DOWN)
        player->facing_dir=0;
        actor_turn(player);
    }
    SPR_setPosition(player->sprite,WINDOW_X+player->x * 16 + player->scroll_x, WINDOW_Y+player->y * 16 - 4 + player->scroll_y);
    if (gm_timer==60)
    {
        transition_start();
    }
}

void game_update()
{
    switch (gm_state)
    {
        case GAME_STATE_NORMAL:
        game_check_turn();
        break;

        case GAME_STATE_MOVE:
        game_run_move();
        break;

        case GAME_STATE_GATE:
        game_run_gate();
        break;
    }
    
}