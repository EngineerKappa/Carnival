#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <background.h>
#include <input.h>
#include <actors.h>




int main()
{
	JOY_init();
	JOY_setEventHandler(&input_update);
	
	SPR_init();
	XGM_startPlay(bgm_cutscene);
	PAL_setPalette(PAL0, frame.palette->data, DMA);
	PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
	input_init();
	BG_init();
	actors_init();

	while(1)
	{        
		run_turn();
		background_x +=1;
		MAP_scrollTo(bgb, background_x / 2,background_y / 2);
		MAP_scrollTo(bga, background_x,background_y);
		SPR_update();
		SYS_doVBlankProcess();
	}
	return (0);
}
