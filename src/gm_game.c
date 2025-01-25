#include <headers.h>


void game_init()
{
    
    room_list_init();
    
    actors_clear_all(true);
    actors_init();
    VDP_loadTileSet(&castle_tileset,BG_VRAM_ind,DMA);
    VRAM_ind+=castle_tileset.numTile;
    PAL_setPalette(PAL1, spr_swordsman.palette->data, DMA);
    PAL_setPalette(PAL2, castle_palette.data, DMA);
    PAL_setPalette(PAL3, palette_red, DMA);
    
    floor_current=1;
    yorb_count=0;
    step_count=0;
    func_update=game_update;
    update_hud=true;
    score=0;
    player_hp=PLAYER_HP_MAX;
    game_clear=false;
    room_load();
    room_init();
    SYS_setVBlankCallback(game_draw_hud_text);
    XGM2_play(bgm_fjf);
}

void game_update_move()
{
    game_pixels_scrolled+=2;
    game_scroll_all_actors();
    if (game_pixels_scrolled>=16)
    {
        game_pixels_scrolled=0;
        if (attacker_count==0)
        {
            gm_state=GAME_STATE_NORMAL;
            game_scroll_end();
            game_check_turn();
        }
        else
        {
            gm_timer=0;
            gm_state=GAME_STATE_ATTACK;
        }   
    }
}



void game_update_attack()
{
    if (gm_timer<15)
    {
        actor_defending_shake=-actor_defending_shake;
        if (actor_defending_shake>0)
        actor_defending_shake--;
        if (actor_defending_shake<0)
        actor_defending_shake++;

        if (actor_defending->facing_dir== DIR_DOWN || actor_defending ->facing_dir == DIR_UP)
        {
            actor_defending->scroll_y=+(dir_get_y(actor_attacking->facing_dir)*3) +actor_defending_shake;
        }
        else
        {
            actor_defending->scroll_x=+(dir_get_x(actor_attacking->facing_dir)*3) +actor_defending_shake;
        }
    }
    
    
    if (gm_timer==0)
    {
        actor_defending_shake=6;
        actor_defending_will_counter=false;
        if (actor_attacking!=NULL)
        {
            actor_attacking->scroll_x=dir_get_x(actor_attacking->facing_dir)*3;
            actor_attacking->scroll_y=dir_get_y(actor_attacking->facing_dir)*3;
        }
        
        
        if (actor_defending==player)
        {
            
            actor_defending_palette=PAL1;
            player_hp-=1;
            XGM2_playPCM(snd_hurt,sizeof(snd_hurt),SOUND_PCM_CH2);
            if (player_hp==0)
            {
                XGM2_stop();
                
            }
            update_hud=true;
        }
        else
        {
            if (actor_attacking!=NULL)
            {
                actor_attacking->frame = 0;
                actor_face_dir(actor_attacking);
            }
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
            if (actor_attacking!=NULL)
            {
                actor_defending->facing_dir=dir_get_180(actor_attacking->facing_dir);
            }
            actor_face_dir(actor_defending);
            enemy_take_damage(actor_defending);
        }

        
    }

    if (gm_timer == 5 && actor_attacking!=NULL)
    {
        actor_attacking->frame = 1;
        actor_face_dir(actor_attacking);
    }

    if (gm_timer == 15 && actor_attacking!=NULL)
    {
        actor_attacking->frame = 0;
        actor_face_dir(actor_attacking);
    }

    if (gm_timer>15 && actor_attacking!=NULL)
    {
        if (actor_attacking->scroll_x>0)
        actor_attacking->scroll_x--;
        if (actor_attacking->scroll_x<0)
        actor_attacking->scroll_x++;
        if (actor_attacking->scroll_y>0)
        actor_attacking->scroll_y--;
        if (actor_attacking->scroll_y<0)
        actor_attacking->scroll_y++;
    }

    if (gm_timer > 20)
    {
        if (actor_defending->scroll_x>0)
        actor_defending->scroll_x--;
        if (actor_defending->scroll_x<0)
        actor_defending->scroll_x++;
        if (actor_defending->scroll_y>0)
        actor_defending->scroll_y--;
        if (actor_defending->scroll_y<0)
        actor_defending->scroll_y++;
    }
    

    if (gm_timer % 10 == 0)
    {
        SPR_setPalette(actor_defending->sprite,PAL3);
    }

    if (gm_timer % 10 == 5)
    {
        SPR_setPalette(actor_defending->sprite,actor_defending_palette);
    }

    if (player_hp==0 && gm_timer == 20)
    {
        SPR_setPalette(actor_defending->sprite,actor_defending_palette);
        gm_timer=0;
        gm_state=GAME_STATE_DEATH;
        SPR_setPosition(actor_defending->sprite,WINDOW_X+actor_defending->x * 16 + actor_defending->scroll_x, WINDOW_Y+actor_defending->y * 16 - 4 + actor_defending->scroll_y);
        SPR_setPosition(actor_attacking->sprite,WINDOW_X+actor_attacking->x * 16 + actor_attacking->scroll_x, WINDOW_Y+actor_attacking->y * 16 - 4 + actor_attacking->scroll_y);
        return;
    }

    if (gm_timer==30)
    {       
        SPR_setPalette(actor_defending->sprite,actor_defending_palette);
        gm_timer=0;
        if (!actor_defending_will_counter)
        {
            
            attacker_count=0;
            if (player_hp>0)
            {
                gm_state=GAME_STATE_NORMAL;
                
            }
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

    SPR_setPosition(actor_defending->sprite,WINDOW_X+actor_defending->x * 16 + actor_defending->scroll_x, WINDOW_Y+actor_defending->y * 16 - 4 + actor_defending->scroll_y);
    SPR_setPosition(actor_attacking->sprite,WINDOW_X+actor_attacking->x * 16 + actor_attacking->scroll_x, WINDOW_Y+actor_attacking->y * 16 - 4 + actor_attacking->scroll_y);
    gm_timer++;
}

void game_end()
{
    VDP_setHilightShadow(false);
    actors_clear_all(true);
    VDP_clearPlane(BG_B,true);
    VDP_clearSprites();
}

void game_update()
{
    switch (gm_state)
    {
        case GAME_STATE_NORMAL:
        
        game_check_turn();
        break;

        case GAME_STATE_MOVE:
        game_update_move();
        break;

        case GAME_STATE_ATTACK:
        game_update_attack();
        break;

        case GAME_STATE_DEATH:
        game_update_death();
        break;

        case GAME_STATE_GATE:
        game_update_gate();
        break;
    }
    if (gm_state!=GAME_STATE_GAMEOVER)
    {
        game_update_actors_realtime();
    }
}

void game_update_gate()
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
    if (gm_timer==60 && !game_clear)
    {
        transition_start(transition_room_next);
    }

    if (gm_timer==120 && game_clear)
    {
        transition_start(transition_game_clear);
    }
}