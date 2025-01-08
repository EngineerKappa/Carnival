#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <system.h>
#include <gm_game.h>
#include <input.h>
#include <actors.h>
#include <background.h>
#include <room.h>


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
    
    func_update=game_update;
    room_init();
    //XGM_startPlay(bgm_fjf);
}

void game_end()
{
    SPR_end();
}

void game_update()
{
    switch (gm_state)
    {
        case GAME_STATE_NORMAL:
        game_check_turn();
        break;

        case GAME_STATE_MOVE:
        game_pixels_scrolled+=2;
        game_move_actors();
        if (game_pixels_scrolled>=16)
        {
            game_pixels_scrolled=0;
            gm_state=GAME_STATE_NORMAL;
            game_move_end();
            game_check_turn();
        }
        break;
    }
    
}