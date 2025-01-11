#include <headers.h>


void game_init()
{
    room_list_init();
    BG_load_frame();
    actors_clear_all(true);
    actors_init();
    VDP_loadTileSet(&castle_tileset,BG_VRAM_ind,DMA);
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    PAL_setPalette(PAL2, castle_palette.data, DMA);
    
    floor_current=3;
    yorb_count=0;
    step_count=0;
    func_update=game_update;
    update_hud=true;
    room_load();
    room_init();
    SYS_setVBlankCallback(game_draw_hud_text);
    XGM2_play(bgm_fjf);
}

void game_end()
{
    SPR_end();
}

void game_run_move()
{
    game_pixels_scrolled+=2;
    game_move_actors();
    if (game_pixels_scrolled>=16)
    {
        game_pixels_scrolled=0;
        gm_state=GAME_STATE_NORMAL;
        game_move_end();
        game_check_turn();
    }

}

void game_draw_hud_labels()
{
    VDP_drawTextBG(BG_B,"Castle",28,2);
    VDP_drawTextBG(BG_B,"  Kranion",28,3);
    VDP_drawTextBG(BG_B,"Swordsman",28,7);
    VDP_drawTextBG(BG_B,"Floor ",28,5);
    VDP_drawTextBG(BG_B,"Yorbs",28,11);
    VDP_drawTextBG(BG_B,"Score",28,23);
}

void game_draw_hud_text()
{
    if (!update_hud)
    return;
    char str[10];
    
    intToStr(floor_current,str,2);
    VDP_drawTextBG(BG_B,str,34,5);

    //HP
    VDP_drawTextBG(BG_B,"*****",30,9);

    //Yorbs
    intToStr(yorb_count,str,3);
    VDP_drawTextBG(BG_B,str,30,13);

    //Score
    intToStr(0,str,3);
    VDP_drawTextBG(BG_B,str,31,25);
    update_hud=false;
}

void game_run_gate()
{
    int play_y;
    if (gm_timer==0)
    {
        gate_speed=0;
        SPR_setPriority(player->sprite,true);
        VDP_setHilightShadow(true);
    }
    
    gm_timer++;
    player->scroll_y-=gate_speed;
    player->timer++;
    if (player->timer == 4)
    {   
        gate_speed+=1;
        player->timer = 0;
        player->facing_dir+=1;
        if (player->facing_dir > DIR_DOWN)
        player->facing_dir=0;
        actor_face_dir(player);
    }
    play_y=WINDOW_Y+player->y * 16 - 4 + player->scroll_y;
    if (play_y<-16)
    play_y=-16;

    SPR_setPosition(player->sprite,WINDOW_X+player->x * 16 + player->scroll_x, play_y);
    if (gm_timer==60)
    {
       transition_start(transition_room_next);
    }
}

void game_update()
{
    switch (gm_state)
    {
        case GAME_STATE_NORMAL:
        
        game_check_turn();
        break;

        case GAME_STATE_MOVE:
        game_run_move();
        break;

        case GAME_STATE_GATE:
        game_run_gate();
        break;
    }
    game_run_actors_realtime();
}