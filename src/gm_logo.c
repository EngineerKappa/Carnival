#include <headers.h>

enum {
    LOGO_STATE_SCROLL = 0,
    LOGO_STATE_FLASH,
    LOGO_STATE_EXIT
};

Sprite* sprite_kappa;
Sprite* sprite_eris;
Sprite* sprite_boxer;

int16_t kappa_x;
int16_t eris_x;

u16 palcycle_AB[6];
Palette pal_logo;
u8 palcycle_wait;
u8 palcycle_index;

void logo_init()
{
    palcycle_index=3;
    //PAL_setColors(PAL2*16,palette_black,16,DMA);
    func_update=logo_update;
    
    kappa_x=-128;
    eris_x=-64;
    gm_state=LOGO_STATE_SCROLL;
    
    
    sprite_kappa=SPR_addSprite(&spr_kappa,kappa_x,150,TILE_ATTR(PAL2,0,FALSE,FALSE));
    sprite_eris=SPR_addSprite(&spr_eris,eris_x,150,TILE_ATTR(PAL3,0,FALSE,FALSE));
    sprite_boxer=SPR_addSprite(&spr_boxer,eris_x,150-24,TILE_ATTR(PAL3,0,FALSE,FALSE));
    VDP_waitVBlank(true);
    VDP_setVerticalScroll(BG_B,-background_y);
    PAL_setPalette(PAL1, palette_engineerkappa_2.data, DMA);
    PAL_setPalette(PAL0, bg_engineerkappa.palette->data, DMA);
    PAL_setPalette(PAL2, spr_kappa.palette->data, DMA);
    PAL_setPalette(PAL3, spr_eris.palette->data, DMA);
    VDP_drawImageEx(BG_B,&bg_engineerkappa,TILE_ATTR_FULL(PAL1, true, false, false, BG_VRAM_ind),6,0,false,true);
}

void logo_skip()
{
    PAL_setPalette(PAL0, frame.palette->data, DMA);
    gm_state=LOGO_STATE_EXIT;
    transition_start(menu_gameover_end_game);
    return;
}

void palcycle_process()
{	
	if (palcycle_wait == 0)
	{
        palcycle_wait=3;
        u8 ind=0;
        u16 col;
        for (size_t i = 0; i < 17; i++)
        {
            if (i>=3)
            {
                ind=i+palcycle_index;
                if (ind>=16)
                ind-=13;

                col=PAL_getColor(PAL0*16+ind);
                PAL_setColor(PAL1*16+i, col);
            }
            else
            {
                col=PAL_getColor(PAL0*16+i);
                PAL_setColor(PAL1*16+i, col);
            }
        }
		
		palcycle_index++;
		if (palcycle_index>=13)
		palcycle_index=0;
	}
	else
	{
		palcycle_wait--;
	}
}

void logo_end()
{
    background_y=0;
    VDP_clearPlane(BG_B,true);
    VDP_setVerticalScroll(BG_B,0);
    SPR_releaseSprite(sprite_kappa);
    SPR_releaseSprite(sprite_eris);
    SPR_releaseSprite(sprite_boxer);
    SPR_defragVRAM();
}

void logo_update()
{
    if (gm_state!=LOGO_STATE_EXIT)
    {
        if (joy_pressed(BUTTON_START))
        {
            logo_skip();
        }
    }
    if (gm_state==LOGO_STATE_SCROLL)
    {
        
        background_y+=2;
        if (background_y>=96)
        {
            background_y=96;
            gm_state=LOGO_STATE_FLASH;
            
            //PAL_setPalette(PAL1, bg_engineerkappa.palette->data, DMA);
            VDP_drawTextBG(BG_B,"Presents...",15,3);
            XGM2_play(bgm_engineerkappa);
        }

        VDP_setVerticalScroll(BG_B,-background_y);
    }

    if (gm_state==LOGO_STATE_FLASH)
    {
       
        if (kappa_x < 400)
        {
            kappa_x+=3;
            
        }
        else
        {
            logo_skip();
            return;
        }

        if (eris_x < 400)
        eris_x+=4;

        SPR_setPosition(sprite_kappa,kappa_x,150);
        SPR_setPosition(sprite_eris,eris_x,150);
        SPR_setPosition(sprite_boxer,eris_x-16 + (sinFix16(eris_x*2) / 16),150 + (sinFix16(eris_x*6) / 16));
        VDP_waitVBlank(false);
        palcycle_process();
    }
}