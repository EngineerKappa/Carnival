#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <background.h>
#include <input.h>
int main()
{
	SPR_init();
	input_init();
	BG_init();
	PAL_setPalette(PAL0, frame.palette->data, DMA);
	loadBackground(bg_nohbodee);
	//VDP_drawImage(BG_B,,0,0);
	
	
	VDP_loadFont(custom_font.tileset, DMA);
	VDP_setTextPlane(BG_B);
	XGM_startPlay(bgm_cutscene);
	VDP_drawText("When the Super Freaks reached", 2, 2);
	VDP_drawText("Freaky World, they received", 2, 3);
	VDP_drawText("a psychic message.", 2, 4);
	while(1)
	{        
		VDP_waitVSync();
	}
	return (0);
}
