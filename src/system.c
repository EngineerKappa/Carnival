#include <headers.h>

char *str_imfreaks = "im freaks. ";
char *str_lungs = "the lungs will diffuse it into your bloodstream and carry it around your muscles to replenish them";

void sram_save()
{
	u32 sram_ind=0;
	u16 checksum=0;

	for (size_t i = 0; i < strlen(str_lungs); i++)
	{
		checksum+=*(str_lungs + i);
		SRAM_writeByte(sram_ind,*(str_lungs + i));
		sram_ind+=1;
	}

	SRAM_writeByte(sram_ind,floors_cleared);
	sram_ind+=sizeof(u8);
	checksum+=floors_cleared;

	SRAM_writeWord(sram_ind,score_best);
	sram_ind+=sizeof(u16);
	checksum+=score_best;

	SRAM_writeWord(sram_ind,checksum);
	sram_ind+=sizeof(u16);

	
	sram_ind++;
	while(sram_ind<512)
	{
		for (size_t i = 0; i < strlen(str_imfreaks); i++)
		{
			SRAM_writeByte(sram_ind,*(str_imfreaks + i));
			sram_ind+=1;
		}
	}
}

void sram_load()
{
	u8 sram_ind=0;
	u16 checksum=0;
	u8 stc;
	u8 sr_floors_cleared;
	u16 sr_score_best;
	u16 sr_checksum;

	for (size_t i = 0; i < strlen(str_lungs); i++)
	{
		stc=SRAM_readByte(sram_ind);
		if (stc==*(str_lungs + i))
		{
			sram_ind+=1;
			checksum+=stc;
		}
		else
		{
			//VDP_setHilightShadow(true); //Fail Checksum
			return;
		}
	}

	sr_floors_cleared=SRAM_readByte(sram_ind);
	checksum+=sr_floors_cleared;
	sram_ind+=sizeof(u8);

	sr_score_best=SRAM_readWord(sram_ind);
	checksum+=sr_score_best;
	sram_ind+=sizeof(u16);

	sr_checksum=SRAM_readWord(sram_ind);
	if (checksum==sr_checksum)
	{
		//Success!
		score_best=sr_score_best;
		floors_cleared=sr_floors_cleared;
	}
	else
	{

		//VDP_drawTextBG(BG_B,"Checksum failed",1,1);
		//char str[10];
    	//intToStr(checksum,str,0);
		//VDP_drawText(str,1,2);
		//VDP_drawText("calculated",1,3);
    	//intToStr(sr_checksum,str,0);
		//VDP_drawText(str,1,4);
		//VDP_drawText("found in SRAM",1,5);

		//VDP_waitVBlank(true);
		//waitMs(400);
	}


	
}

void system_init()
{
    menu_init();
	input_init();
	SRAM_enable();
	SPR_init();
	BG_init();
	Z80_loadDriver(Z80_DRIVER_XGM2,true);
	XGM2_setLoopNumber(-1);
	transition_init();
	VDP_waitVBlank(true);
    PAL_setPalette(PAL0, frame.palette->data, DMA);
	VDP_loadFont(custom_font.tileset, DMA);
	BG_load_frame();
	room_list_init();
    gm_current=DEBUG_GM;
	floors_cleared=0;
	floor_current=DEBUG_FLOOR;
	text_init();
	sram_load();
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