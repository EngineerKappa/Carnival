#include <headers.h>


void player_init(){
    //The player should be spawned first
    Actor *pl = &actors[0];
   
    
    pl->act_move_start = player_step;
    pl->act_move_finish = player_collect_item;
    actors_spawned=1;
    player = pl;
    pl->sprite = SPR_addSprite(&spr_swordsman,WINDOW_X+pl->x * 16 ,WINDOW_Y+pl->y * 16,TILE_ATTR(PAL1,0,FALSE,pl->hflip));
    player_reset();
}

void player_reset()
{
    player->x = 0;
    player->y = 0;
    player->type = OBJ_PLAYER;
    player->state=0;
    player->frame=0;
    player->facing_dir = DIR_DOWN;
    player->hflip = false;
    player->vflip = false;
    player->scroll_x = 0;
    player->scroll_y = 0;
    SPR_setVFlip(player->sprite,false);
    SPR_setPriority(player->sprite,false);
    SPR_setDepth(player->sprite,SPR_MIN_DEPTH);
    actor_face_dir(player);
    actor_move_forward(player);

}

void player_step()
{
    
    //player_collect_item();
}

void player_collect_item()
{
    u8 i;
    u8 px=player->x;
    u8 py=player->y;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        
        Actor* a = &actors[i];
        if ( a->x == px && a->y == py)
        {
            switch (a->type)
            {
            case OBJ_YORB:
                yorb_collect(a);
                break;
            case OBJ_HEART:
                heart_collect(a);
                break;
            case OBJ_GATE:
                XGM2_playPCM(snd_teleport,sizeof(snd_teleport),SOUND_PCM_CH2);
                gm_timer=0;
                score+=1000;
                update_hud=true;
                gm_state=GAME_STATE_GATE;


                if (floor_current>=MAX_ROOMS)
                {
                    game_clear=true;
                    XGM2_stop();
                }

                break;
            }
            
        }
    }
}