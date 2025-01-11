#include <headers.h>

void system_init()
{
    JOY_init();
	input_init();
	SPR_init();
	BG_init();
	Z80_loadDriver(Z80_DRIVER_XGM2,true);
	XGM2_setLoopNumber(0);
	transition_init();
	JOY_setEventHandler(&input_update);
    PAL_setPalette(PAL0, frame.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
    gm_current=GM_TITLE;
	
	gm_start(gm_current);
}

void system_update()
{
	if (func_update!=NULL && transition_state==0)
	func_update();

	
}

void gm_end()
{
	switch (gm_current) 
	{
		case GM_TITLE:
			title_end();
			break;
	}
}

void gm_change(u8 gm)
{
	gm_end();
	gm_start(gm);
}

void gm_start(u8 gm)
{
	gm_current=gm;
	gm_state=0;
	gm_timer=0;
    switch (gm) 
	{
		case GM_TITLE:
			title_init();
			break;
		
		case GM_GAME:
			game_init();
			break;
	}
}