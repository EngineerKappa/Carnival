#include <headers.h>

void system_init()
{
    menu_init();
	input_init();
	
	SPR_init();
	BG_init();
	Z80_loadDriver(Z80_DRIVER_XGM2,true);
	XGM2_setLoopNumber(-1);
	transition_init();
	VDP_waitVBlank(true);
    PAL_setPalette(PAL0, frame.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
	BG_load_frame();
    gm_current=DEBUG_GM;
	text_init();
	
	gm_start(gm_current);
}

void system_update()
{
	if (func_update!=NULL && transition_state==0)
	func_update();

	if (menu_state==MENU_STATE_ACTIVE)
	{
		menu_update();
	}
	if (textbox_state==TEXT_STATE_TYPING)
	{
		text_update();
	}
	
}

void gm_end()
{
	switch (gm_current) 
	{
		case GM_LOGO:
			logo_end();
			break;
		case GM_TITLE:
			title_end();
			break;
		case GM_GAME:
			game_end();
			break;
		case GM_ENDING:
			ending_end();
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
		case GM_LOGO:
			logo_init();
			break;

		case GM_TITLE:
			title_init();
			break;
		
		case GM_GAME:
			game_init();
			break;
		
		case GM_ENDING:
			ending_init();
			break;
	}
}