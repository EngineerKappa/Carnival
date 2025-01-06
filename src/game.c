#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <input.h>
#include <actors.h>
#include <background.h>


void game_init()
{
    input_init();
    BG_init();
    JOY_setEventHandler(&input_update);
    PAL_setPalette(PAL0, frame.palette->data, DMA);
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    actors_init();
}