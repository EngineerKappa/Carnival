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
    room_list_init();
    BG_load_frame();
    actors_clear_all(true);
    actors_init();
    VDP_loadTileSet(&castle_tileset,BG_VRAM_ind,DMA);
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    PAL_setPalette(PAL2, castle_palette.data, DMA);
    
    floor_current=1;
    yorb_count=0;
    step_count=0;
    func_update=game_update;
    room_load();
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
    char str[10];
    VDP_drawTextBG(BG_B,"Castle",28,2);
    VDP_drawTextBG(BG_B,"  Kranion",28,3);

    sprintf(str, "Floor %d", floor_current);
    VDP_drawTextBG(BG_B,str,28,5);

    VDP_drawTextBG(BG_B,"Swordsman",28,7);
    VDP_drawTextBG(BG_B,"*****",30,9);

    VDP_drawTextBG(BG_B,"Yorbs",28,11);
    sprintf(str, "%02d / 50", yorb_count);
    VDP_drawTextBG(BG_B,str,30,13);

    VDP_drawTextBG(BG_B,"Treasure",28,15);
    VDP_drawTextBG(BG_B,"000",31,17);

    sprintf(str, "%04d", step_count);
    VDP_drawTextBG(BG_B,"Steps",28,19);
    VDP_drawTextBG(BG_B,str,31,21);

    VDP_drawTextBG(BG_B,"Score",28,23);
    VDP_drawTextBG(BG_B,"00000",31,25);

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
       transition_start(transition_room_next);
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
    game_run_actors_realtime();
}