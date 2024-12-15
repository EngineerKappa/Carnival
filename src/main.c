#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <background.h>
#include <input.h>



int main()
{
	XGM_startPlay(bgm_cutscene);
	PAL_setPalette(PAL0, frame.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
	BG_init();

	
	
	
	
	
	
	while(1)
	{        
		background_x +=1;

		MAP_scrollTo(bgb, background_x / 2,background_y / 2);
		MAP_scrollTo(bga, background_x,background_y);
		SYS_doVBlankProcess();
	}
	return (0);
}
