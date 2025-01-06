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
	PAL_setPalette(PAL0, frame.palette->data, DMA);
	PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
	input_init();
	BG_init();
	actors_init();
	//XGM_startPlay(bgm_cutscene);

	while(1)
	{        
		run_turn();
		MAP_scrollTo(bga, 0,0);
		SPR_update();
		SYS_doVBlankProcess();
	}
	return (0);
}
