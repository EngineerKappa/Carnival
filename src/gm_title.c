#include <headers.h>
void title_draw_swordsman()
{
    PAL_setPalette(PAL1, bg_titleswordsman.palette->data, DMA);
    VDP_drawImageEx(BG_B,&bg_titleswordsman,TILE_ATTR_FULL(PAL1, true, false, false, BG_VRAM_ind),2,1,false,true);
}

void title_draw()
{
    
    VDP_drawTextBG(BG_B,"Super Freaks",25,7);
    VDP_drawTextBG(BG_B,"Presents...",27,8);
    VDP_drawTextBG(BG_B,"SWORDSMAZE",26,13);

    char str[10];
    intToStr(score_best,str,5);
    VDP_drawTextBG(BG_B,"Best Score: ",22,1);
    VDP_drawTextBG(BG_B,str,34,1);

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
    title_draw_swordsman();
    title_draw();
    XGM2_play(bgm_title);
    XGM2_setLoopNumber(-1);
    func_update=title_update;
}

void menu_title_update_floor_select()
{
    char str[10];
    intToStr(floor_current,str,2);
    VDP_drawTextBG(BG_A,str,22,19);
}

void menu_title_floor_select()
{
    gm_state=TITLE_STATE_FLOORSELECT;
    menu_clear();
    VDP_drawTextBG(BG_A,"Floor Select",14,17);

    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, TRUE, UI_VRAM_ind + 3),14,19);
    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, UI_VRAM_ind + 3),25,19);
    VDP_drawTextBG(BG_A,"Floor",16,19);
    menu_title_update_floor_select();

    VDP_drawTextBG(BG_A,"START - Select    B - Back",7,22);
    VDP_clearTextLineBG(BG_B,24);
}

void title_update()
{
    if (gm_state==TITLE_STATE_NORMAL)
    {
        if (gm_timer==5)
        {
            menu_create(15,17);
            menu_add_option("START GAME", menu_title_start_game);
            if (floors_cleared>0)
            {
                menu_add_option("PICK FLOOR", menu_title_floor_select);
            }
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
            title_draw_swordsman();
            title_draw();
            gm_state=TITLE_STATE_NORMAL;
        }
    }
    if (gm_state==TITLE_STATE_FLOORSELECT)
    {
        if (joy_pressed(BUTTON_LEFT))
        {
            if (floor_current>1)
            {
                floor_current--;
            }
            else
            {
                floor_current=min(MAX_ROOMS,floors_cleared+1);
            }
            menu_title_update_floor_select();
            return;
        }

        if (joy_pressed(BUTTON_RIGHT))
        {   
            if (floor_current<min(MAX_ROOMS,floors_cleared+1))
            {
                floor_current++;
            }
            else
            {
                floor_current=1;
            }
            menu_title_update_floor_select();
            return;
        }

        if (joy_pressed(BUTTON_START))
        {
            menu_title_start_game();
            XGM2_playPCM(snd_ding,sizeof(snd_ding),SOUND_PCM_CH2);
            return;
        }
        
        if (joy_pressed(BUTTON_B))
        {
            gm_timer=0;
            menu_clear();
            VDP_clearTextLineBG(BG_A,22);
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
    if (gm_state!=TITLE_STATE_FLOORSELECT)
    {
        floor_current=DEBUG_FLOOR;
    }
    menu_option_previous=0;
    gm_change(GM_GAME);
}