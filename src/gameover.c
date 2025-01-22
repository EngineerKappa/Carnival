#include <headers.h>
void game_update_death()
{
    if (gm_timer==0)
    {
        XGM2_playPCM(snd_wilhelm,sizeof(snd_wilhelm),SOUND_PCM_CH2);
    }

    if (player_hp==0 && gm_timer < 30 && (gm_timer % 2 == 0))
    {
        player->facing_dir++;
        actor_face_dir(player);
    }

    if (gm_timer==30)
    {
        SPR_setAnim(player->sprite,2);
        SPR_setVFlip(player->sprite,true);
    }

    if (gm_timer==60)
    {
        SPR_setAnim(player->sprite,2);
        SPR_setVFlip(player->sprite,true);
        PAL_fadeOutPalette(PAL2,30,true);       
    }

    if (gm_timer==90)
    {
        actors_clear_all(false);
        
        VDP_drawImageEx(BG_A,&bg_gameover,TILE_ATTR_FULL(PAL0, true, false, false, VRAM_ind),3,2,false,true);
        
        if (random() % 5 > 1)
        {
            XGM2_play(bgm_gameover1);
        }
        else
        {
            XGM2_play(bgm_gameover2);
        }
    }

    if (gm_timer == 120)
    {
        gm_state=GAME_STATE_GAMEOVER;
        menu_create(9,18);
        menu_add_option(" CONTINUE ", menu_gameover_continue);
        menu_add_option(" END GAME ", menu_gameover_end_game);
        menu_draw(false);
    }

    gm_timer++;
}

void menu_gameover_continue()
{
    XGM2_stop();
    score=0;
    transition_start(transition_gameover_continue);
}

void menu_gameover_end_game()
{
    XGM2_stop();
    transition_start(transition_gameover_end_game);
}

void transition_gameover_continue()
{
    PAL_setPalette(PAL2, castle_palette.data, DMA);
    player_hp=PLAYER_HP_MAX;
    room_end(true);
    room_init();
    XGM2_play(bgm_fjf);
}

void transition_gameover_end_game()
{
    gm_change(GM_TITLE);
}