#include <headers.h>

void title_init()
{
    VDP_drawTextBG(BG_B,"SUPER FREAKS PRESENTS: SWORDSMAZE",3,5);

    VDP_drawTextBG(BG_B,"Game by EngineerKappa",2,24);
    VDP_drawTextBG(BG_B,"Characters by SuperFreaksDev",2,25);
    VDP_drawTextBG(BG_B,"Made for FreakJam 2025",2,26);
    XGM2_play(bgm_title);
    func_update=title_update;
}

void title_update()
{
    gm_timer++;
    if (gm_timer==15)
    {
        VDP_drawTextBG(BG_B,"PUSH START BUTTON",11,18);
    }
    if (gm_timer==30)
    {
        VDP_clearTextAreaBG(BG_B,11,18,17,1);
        gm_timer=0;
    }
    
    if (joypad_data & BUTTON_START)
    {
        transition_start(transition_title_start_game);
        XGM2_stop();
        
        XGM2_playPCM(snd_ding,sizeof(snd_ding),SOUND_PCM_CH2);
    }
}

void title_end()
{
    
    VDP_clearTextAreaBG(BG_B,0,0,40,30);
}