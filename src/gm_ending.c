#include <headers.h>

//0G,1A,2M,3E,4C,5L,3E,1A,6R
u8 game_clear_letters[9]={0,1,2,3,4,5,3,1,6};
u8 game_clear_letter_width[7]={20,20,27,20,20,19,23};
u16 scale;
u16 sinewave;
u8 final_hp;
u8 hp_counted;

void ending_init()
{
    XGM2_play(bgm_clear);
    VDP_fillTileMapRect(BG_B,8,0,0,40,30);
    actors_clear_all(true);
    actors_spawned=0;
    gm_state=ENDING_STATE_ENTER;
    func_update=ending_update;
    s16 x,y;
    x=50;
    y=88;
    scale=128;
    sinewave=0;
    hp_counted=0;
    final_hp=0;
    for (size_t i = 0; i < 9; i++)
    {
        actors[i].sprite=SPR_addSprite(&spr_uitext,-32,-32,TILE_ATTR(PAL0,false,false,false));
        actors[i].scroll_x=x;
        actors[i].scroll_y=y;
        actors[i].type=OBJ_EFFECT;
        if (i<8)
        {
            x+=(game_clear_letter_width[game_clear_letters[i]] / 2);
            x+=(game_clear_letter_width[game_clear_letters[i+1]] / 2);
        }
        
        if (i==3)
        x+=16;

        SPR_setAnim(actors[i].sprite,game_clear_letters[i]);
        actors_spawned++;
    }
    
}

void ending_update_score()
{
    char str[10];
    intToStr(score,str,5);
    VDP_drawTextBG(BG_A,str,23,23);
}

void ending_update()
{
    s16 x,y;
    s8 flip;
    flip=1;
    if (gm_state==ENDING_STATE_ENTER)
    {
        if (scale>0)
        {
            scale-=8;
        }
        else
        {
            gm_timer++;

            if (gm_timer>30)
            {
                if (background_y>=-58)
                {
                    background_y-=3;
                }
                else
                {
                    gm_timer=0;
                    gm_state=ENDING_STATE_TALLY;
                    
                }
            }
        }
    }

    if (gm_state==ENDING_STATE_TALLY)
    {
        if (gm_timer==0)
        {
            
            VDP_fillTileMapRect(BG_B,12,0,22,40,3);
            VDP_drawTextBG(BG_A,"Final Score:",10,23);
            ending_update_score();
            
        }

        if (gm_timer==30)
        {
            if (hp_counted<PLAYER_HP_MAX)
            {
                if (hp_counted==0)
                {
                    VDP_fillTileMapRect(BG_B,12,0,10,40,4);
                }
                gm_timer=1;
                if (hp_counted<(player_hp))
                {
                    XGM2_playPCM(snd_heart,sizeof(snd_heart),SOUND_PCM_CH3);
                    VDP_fillTileMapRectInc(BG_B,UI_VRAM_ind+6,5+(hp_counted*3),11,2,2);
                    score+=100;
                    ending_update_score();
                }
                else
                {
                    XGM2_playPCM(snd_hurt,sizeof(snd_hurt),SOUND_PCM_CH3);
                    VDP_fillTileMapRectInc(BG_B,UI_VRAM_ind+10,5+(hp_counted*3),11,2,2);
                }
                hp_counted++;
            }
        }

        if (gm_timer>30)
        {
            if (joy_pressed(BUTTON_START))
            {
                game_clear=false;
                transition_start(transition_gameover_end_game);
            }
        }
        gm_timer++;
    }

    for (size_t i = 0; i < 9; i++)
    {
        x=actors[i].scroll_x;
        if (i<4)
            x-=scale;
        else
            x+=scale;

        y=actors[i].scroll_y+((sinFix16((i*90) + (sinewave*20)) / 12)+(scale*flip)) + background_y;
        SPR_setPosition(actors[i].sprite,x,y);
        flip=-flip;
    }
    sinewave++;
}

void ending_end()
{
    background_y=0;
    actors_clear_all(true);
    SPR_defragVRAM();
    VDP_clearPlane(BG_B,true);
    XGM2_stop();
}

void transition_game_clear()
{
    gm_change(GM_ENDING);
}