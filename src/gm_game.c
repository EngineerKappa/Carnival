#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <system.h>
#include <gm_game.h>
#include <input.h>
#include <actors.h>
#include <background.h>


void game_init()
{
    //BG_display_frame();
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    actors_init();
    func_update=game_update;
    XGM_startPlay(bgm_fjf);
}

void game_update()
{
    run_turn();
}