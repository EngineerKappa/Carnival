#include <headers.h>

void title_draw()
{
    PAL_setPalette(PAL1, bg_titleswordsman.palette->data, DMA);
    VDP_drawImageEx(BG_B,&bg_titleswordsman,TILE_ATTR_FULL(PAL1, true, false, false, BG_VRAM_ind),10,2,false,true);
    VDP_drawTextBG(BG_B,"Super Freaks",3,1);
    VDP_drawTextBG(BG_B,"Presents...",6,2);
    VDP_drawTextBG(BG_B,"SWORDSMAZE",24,16);

    VDP_drawTextBG(BG_B,"Game by EngineerKappa",2,24);
    VDP_drawTextBG(BG_B,"Super Freaks by SuperFreaksDev",2,25);
    VDP_drawTextBG(BG_B,"Made for FreakJam 2025",2,26);
}

void credits_draw()
{
    u8 i=2;
    PAL_setPalette(PAL1, bg_creditskappa.palette->data, DMA);
    VDP_drawImageEx(BG_B,&bg_creditskappa,TILE_ATTR_FULL(PAL1, true, false, false, BG_VRAM_ind),31,19,false,true);

    VDP_drawTextBG(BG_B,"SWORDSMAZE CREDITS",4,i);

    i+=3;
    VDP_drawTextBG(BG_B,"Art, Code, and Game Design:",2,i);
    VDP_drawTextBG(BG_B,"EngineerKappa",20,i+1);
    i+=4;
    VDP_drawTextBG(BG_B,"Super Freaks Characters and World:",2,i);
    VDP_drawTextBG(BG_B,"SuperFreaksDev and Kingnormal",8,i+1);
    i+=4;
    VDP_drawTextBG(BG_B,"Super Freaks Original BGM: philRaco",2,i);
    VDP_drawTextBG(BG_B,"(Sorry for the weird conversions!!)",2,i+1);
    i+=4;
    VDP_drawTextBG(BG_B,"SGDK by Stephane Dallongeville",2,i);
    VDP_drawTextBG(BG_B,"and all its contributors",4,i+1);
    i+=4;
    VDP_drawTextBG(BG_B,"Midi to VGM Conversion tool",2,i);
    VDP_drawTextBG(BG_B,"Nineko's MIDI2VGM",4,i+1);
    


    VDP_drawTextBG(BG_B,"Press B Button to return",4,26);
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, TRUE, FALSE, TRUE, UI_VRAM_ind + 3),2,26);
}

void title_init()
{
    gm_state=TITLE_STATE_NORMAL;
    title_draw();
    XGM2_play(bgm_title);
    XGM2_setLoopNumber(-1);
    func_update=title_update;
}

void title_update()
{
    if (gm_state==TITLE_STATE_NORMAL)
    {
        if (gm_timer==5)
        {
            menu_create(15,19);
            menu_add_option("START GAME", menu_title_start_game);
            menu_add_option(" CREDITS  ", menu_title_credits);
            menu_draw(false);
        }
    }
    if (gm_state==TITLE_STATE_CREDITS)
    {
        if (joy_pressed(BUTTON_B))
        {
            gm_timer=0;
            VDP_clearPlane(BG_B,true);
            XGM2_playPCM(snd_ding,sizeof(snd_ding),SOUND_PCM_CH2);
            title_draw();
            gm_state=TITLE_STATE_NORMAL;
        }
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

void menu_title_credits()
{
    VDP_clearPlane(BG_B,true);
    menu_clear();
    menu_option_previous=menu_option;
    gm_timer=0;
    gm_state=TITLE_STATE_CREDITS;
    menu_state=MENU_STATE_INACTIVE;
    credits_draw();
}

void transition_title_start_game()
{
    menu_option_previous=0;
    gm_change(GM_GAME);
}