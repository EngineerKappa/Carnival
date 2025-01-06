#include <genesis.h>
#include <sound.h>
#include <gfx.h>
#include <game.h>
#include <background.h>
#include <input.h>
#include <actors.h>

int main()
{
	JOY_init();
	SPR_init();
	VDP_loadFont(custom_font.tileset, DMA);
	
	game_init();

	while(1)
	{        
		run_turn();
		SPR_update();
		SYS_doVBlankProcess();
	}
	return (0);
}
