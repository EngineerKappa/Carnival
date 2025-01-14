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
    PAL_setPalette(PAL3, palette_red, DMA);
    
    floor_current=2;
    yorb_count=0;
    step_count=0;
    func_update=game_update;
    update_hud=true;
    player_hp=PLAYER_HP_MAX;
    room_load();
    room_init();
    SYS_setVBlankCallback(game_draw_hud_text);
    XGM2_play(bgm_fjf);
}

void game_run_move()
{
    game_pixels_scrolled+=2;
    game_move_actors();
    if (game_pixels_scrolled>=16)
    {
        game_pixels_scrolled=0;
        if (attacker_count==0)
        {
            gm_state=GAME_STATE_NORMAL;
            game_move_end();
            game_check_turn();
        }
        else
        {
            gm_timer=0;
            gm_state=GAME_STATE_ATTACK;
        }   
    }
}

void game_run_attack()
{
    if (gm_timer==0)
    {
        actor_defending_will_counter=false;
        
        
        if (actor_defending==player)
        {
            XGM2_playPCM(snd_hurt,sizeof(snd_hurt),SOUND_PCM_CH2);
            actor_defending_palette=PAL1;
        }
        else
        {
            XGM2_playPCM(snd_hit,sizeof(snd_hit),SOUND_PCM_CH2);
            actor_defending_palette=PAL0;
            u8 damage=2;

            if (actor_defending->facing_dir==player->facing_dir)
            damage=4;
            if (actor_defending->facing_dir==dir_get_180(player->facing_dir))
            {
                damage=1;
                actor_defending_will_counter=true;
            }


            if (actor_defending->hp<damage)
            {
                actor_defending->hp=0;
            }
            else
            {
                actor_defending->hp-=damage;
            }
            actor_defending->facing_dir=dir_get_180(actor_attacking->facing_dir);
            actor_face_dir(actor_defending);
            enemy_take_damage(actor_defending);
        }
    }


    if (gm_timer % 10 == 0)
    {
        SPR_setPalette(actor_defending->sprite,PAL3);
    }

    if (gm_timer % 10 == 5)
    {
        SPR_setPalette(actor_defending->sprite,actor_defending_palette);
    }

    if (gm_timer==30)
    {
        SPR_setPalette(actor_defending->sprite,actor_defending_palette);
        gm_timer=0;
        if (!actor_defending_will_counter)
        {
            attacker_count=0;
            gm_state=GAME_STATE_NORMAL;
        }
        else
        {
            actor_defending_will_counter=false;
            if (actor_defending->act_counterattack!=NULL)
            {
                actor_defending->act_move_finish(actor_defending);
                return;
            }
        }
    }

    gm_timer++;
}

void game_end()
{
    SPR_end();
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

        case GAME_STATE_ATTACK:
        game_run_attack();
        break;

        case GAME_STATE_GATE:
        game_run_gate();
        break;
    }

    game_run_actors_realtime();
}

void game_run_gate()
{
    int16_t play_y;
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