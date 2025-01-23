#include <headers.h>

void enemy_take_damage(Actor * a)
{
    if (a->hp<=0)
    {
        XGM2_playPCM(snd_zip,sizeof(snd_zip),SOUND_PCM_CH3);
        score+=500;
        update_hud=true;
        actor_defending_will_counter=false;
        actor_clear_blockmap(a);
        a->state=0;
        a->timer=0;
        a->type=OBJ_EFFECT;
        a->act_move_finish=NULL;
        a->act_move_start=NULL;
        a->act_realtime=effect_update;
    }
}

void spawn_pointy(int spawn_x, int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    a->type = OBJ_POINTY;
    a->x = spawn_x;
    a->y = spawn_y;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->facing_dir=facing_dir;
    a->act_move_start=pointy_update;
    a->sprite = SPR_addSprite(&spr_pointy,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrameChangeCallback(a->sprite, &pointy_animate);
    pointy_animate(a->sprite);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH+1);
    actors_spawned++;
}

void pointy_update(Actor * a)
{
    int8_t target_x,target_y;
    target_x=a->x+dir_get_x(a->facing_dir);
    target_y=a->y+dir_get_y(a->facing_dir);

    if (tile_check_wall(target_x,target_y,true))
    {
        a->facing_dir=dir_get_180(a->facing_dir);
        a->target_x=a->x+dir_get_x(a->facing_dir);
        a->target_y=a->y+dir_get_y(a->facing_dir);
    }
    else
    {
        a->target_x=target_x;
        a->target_y=target_y;
    }
}

void pointy_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_pointy[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}

void spawn_boneym(int spawn_x,int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);

    a->type = OBJ_BONEYM;
    a->x = spawn_x;
    a->y = spawn_y;
    a->hp = 4;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->facing_dir=facing_dir;
    a->act_move_start=boneym_turn_start;
    a->act_move_finish=boneym_attack;
    a->act_counterattack=boneym_attack;
    a->sprite = SPR_addSprite(&spr_boneym,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    
    SPR_setAutoTileUpload(a->sprite, FALSE);
    actor_face_dir(a);
    SPR_setFrameChangeCallback(a->sprite, &boneym_animate);
    boneym_animate(a->sprite);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH+1);
    actors_spawned++;
}

void boneym_turn_start(Actor * a)
{
    if (a->x+dir_get_x(a->facing_dir)==player->x && a->y+dir_get_y(a->facing_dir)==player->y)
    {
        return;
    }
    else
    {
        boneym_target_player(a);
    }
}

void boneym_target_player(Actor * a)
{
    u16 min_dist=1000;
    int8_t target_x,target_y;

    int8_t px = player->x;
    int8_t py = player->y;
    int8_t ax = a->x;
    int8_t ay = a->y;
    u8 new_dir = 5;
    u16 dir_dist[4];
    
   // target_dist=((ax+1-px)*(ax+1-px))+((ay-py)*(ay-py));   
    u8 dir_check=1;

    //Loop through each direction multiple times so we can also compare the last few dirs with the first few ones
    for (u8 i = 1; i < 8; i++) 
    {
        if (dir_check==dir_get_180(a->facing_dir))
        {
            dir_check++;
            continue;
        }

        if (dir_check==5)
            dir_check=1;
        target_x=ax+dir_get_x(dir_check);
        target_y=ay+dir_get_y(dir_check);

        //If the Player is nearby, skip everything else, we don't want the player to be seen as a wall
        if (i < 5)
        {
            if ( (player->x==target_x && player->y==target_y) | (player->target_x==target_x && player->target_y==target_y) )
            {
                if (a->facing_dir!=dir_check)
                {
                    a->state=1;
                    a->facing_dir=dir_check;
                    actor_face_dir(a);
                }
                actor_set_blockmap(a);
                return;
            }
        }

        if (!tile_check_wall(target_x,target_y,true))
        {
            dir_dist[dir_check]=((target_x-px)*(target_x-px))+((target_y-py)*(target_y-py));
            if (dir_dist[dir_check]<=min_dist)
            {
                min_dist=dir_dist[dir_check];
                new_dir=dir_check;
            }
        }
        dir_check++;
    }

    if (new_dir!=5)
    {
        
        if (a->facing_dir!=new_dir)
        {
            a->facing_dir=new_dir;
            a->target_x=a->x;
            a->target_y=a->y;
            a->state=1;
        }
        else
        {
            if (!actor_target_forward(a))
            {
                a->facing_dir++;
                if (a->facing_dir==5)
                a->facing_dir=1;
            }
        }
    }
    else
    {
        if (!actor_target_forward(a))
        {
            a->facing_dir++;
            if (a->facing_dir==5)
            a->facing_dir=1;
            a->state=1;
        }
        
    }
    actor_face_dir(a);
    actor_set_blockmap(a);
}

void boneym_attack(Actor * a)
{
    //If the Boney just turned, don't attack immediately
    if (a->state==1)
    {
        a->state=0; 
        return;
    }
    if (!(a->x+dir_get_x(a->facing_dir)==player->x && a->y+dir_get_y(a->facing_dir)==player->y))
    return;

    actor_face_dir(a);
    attacker_count=1;
    actor_attacking=a;
    actor_defending=player;
    gm_timer=0;
    gm_state=GAME_STATE_ATTACK;
}

void boneym_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_boneym[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}