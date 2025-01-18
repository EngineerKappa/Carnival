#include <headers.h>

void title_init()
{
    PAL_setPalette(PAL1, bg_titleswordsman.palette->data, DMA);
    VDP_drawImageEx(BG_B,&bg_titleswordsman,TILE_ATTR_FULL(PAL1, true, false, false, BG_VRAM_ind),10,2,false,true);

    VDP_drawTextBG(BG_B,"Super Freaks",3,1);
    VDP_drawTextBG(BG_B,"Presents...",6,2);
    VDP_drawTextBG(BG_B,"SWORDSMAZE",24,16);

    VDP_drawTextBG(BG_B,"Game by EngineerKappa",2,24);
    VDP_drawTextBG(BG_B,"Characters by SuperFreaksDev",2,25);
    VDP_drawTextBG(BG_B,"Made for FreakJam 2025",2,26);
    XGM2_play(bgm_title);
    XGM2_setLoopNumber(-1);
    func_update=title_update;
    
    
}

void title_update()
{
    if (gm_timer==5)
    {
        menu_create(15,19);
        menu_add_option("START GAME", menu_title_start_game);
        menu_add_option(" CREDITS  ", title_end);
        menu_draw(false);
    }
    
    gm_timer++;
    
}

void title_end()
{
    
    VDP_clearPlane(BG_B,true);
}

void menu_title_start_game()
{
    XGM2_stop();
    transition_start(transition_title_start_game);
}

void transition_title_start_game()
{
    gm_change(GM_GAME);
}