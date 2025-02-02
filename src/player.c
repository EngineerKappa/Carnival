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
    player->collision_layer = BM_LAYER_PLAYER;
    player->collision_mask = BM_MASK_PLAYER;
    SPR_setVFlip(player->sprite,false);
    SPR_setPriority(player->sprite,false);
    SPR_setDepth(player->sprite,SPR_MIN_DEPTH+1);
    actor_face_dir(player);
    actor_target_forward(player);

}

void player_step()
{
    //player_collect_item();
}

void spawn_sword(int spawn_x,int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    a->type = OBJ_EFFECT;
    a->x = spawn_x;
    a->y = spawn_y;
    a->timer = 0;
    a->frame = 0;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->facing_dir=facing_dir;
    a->act_realtime=sword_animate;
    a->sprite = SPR_addSprite(&spr_sword,WINDOW_X+a->x * 16 + (dir_get_x(facing_dir)*12),WINDOW_Y+a->y * 16 - 4 + (dir_get_y(facing_dir)*12),TILE_ATTR(PAL1,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    set_sprite_index(a->sprite, sprite_index_sword);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH);
    SPR_setAlwaysOnTop(a->sprite);
    actors_spawned++;
    switch (facing_dir)
    {
    case DIR_DOWN:
        SPR_setAnim(a->sprite,1);
        
        break;
    case DIR_UP:
        SPR_setAnim(a->sprite,1);
        SPR_setVFlip(a->sprite,true);
        break;
    case DIR_LEFT:
        SPR_setHFlip(a->sprite,true);
        break;
    
    default:
        break;
    }
}

void sword_animate(Actor * a)
{
    a->timer++;
    if (a->timer>3)
    {
        a->frame++;
        a->timer=0;

        if (a->frame>=4)
        {
            actor_free(a);
            return;
        }
    }

    SPR_setFrame(a->sprite,a->frame);
    u16 tileIndex = sprite_index_sword[a->sprite->animInd][a->sprite->frameInd];
    SPR_setVRAMTileIndex(a->sprite,tileIndex);
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
            case OBJ_TROPHY:
                trophy_collect(a);
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