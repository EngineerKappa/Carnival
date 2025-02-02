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

void spawn_fusedropper(int spawn_x, int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    a->type = OBJ_FUSEDROPPER;
    a->x = spawn_x;
    a->y = spawn_y;
    a->hp = 1;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->collision_layer=BM_LAYER_ENEMY;
    a->facing_dir=facing_dir;
    a->act_move_start=fusedropper_update;
    a->sprite = SPR_addSprite(&spr_fusedropper,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrameChangeCallback(a->sprite, &fusedropper_animate);
    fusedropper_animate(a->sprite);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH);
    actors_spawned++;
}

void fusedropper_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_fusedropper[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}

void fusedropper_update(Actor * a)
{
    if (fusedropper_timer==0)
    {
        if (actors_spawned < MAX_ACTORS)
        {
            spawn_barrel(a->x,a->y,a->facing_dir);
        }
    }
}

void spawn_barrel(int spawn_x, int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    a->type = OBJ_BARREL;
    a->x = spawn_x;
    a->y = spawn_y;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->state = 0;
    a->collision_layer=BM_LAYER_HAZARD;
    a->collision_mask=BM_MASK_POINTY;
    a->facing_dir=facing_dir;
    a->hflip = false;
    a->vflip = false;
    a->sprite = NULL;
    a->act_realtime = NULL;
    a->act_move_start=barrel_update;
    a->act_move_finish=barrel_attack;
    a->sprite = SPR_addSprite(&spr_barrel,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrameChangeCallback(a->sprite, &barrel_animate);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH+2);
    actors_spawned++;
}

void barrel_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_barrel[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}

void barrel_update(Actor * a)
{
    if (a->timer>=1)
    {
        actor_free(a);
        return;
    }

    if ((a->facing_dir == DIR_DOWN) | (a->facing_dir == DIR_UP))
    SPR_setAnim(a->sprite,0);
    if ((a->facing_dir == DIR_LEFT) | (a->facing_dir == DIR_RIGHT))
    SPR_setAnim(a->sprite,1);


    int8_t target_x,target_y;
    target_x=a->x+dir_get_x(a->facing_dir);
    target_y=a->y+dir_get_y(a->facing_dir);

    if (blockmap_check(target_x,target_y) & BM_MASK_POINTY)
    {
        a->timer++;
        SPR_setAnim(a->sprite,2);
        XGM2_playPCM(snd_explode,sizeof(snd_explode),SOUND_PCM_CH3);
    }
    else
    {
        a->target_x=target_x;
        a->target_y=target_y;
    }

    if (a->target_x==player->x && a->target_y==player->y && a->x==player->target_x && a->y==player->target_y) //Intercept player movement
    {
        player->target_x=player->x;
        player->target_y=player->y;
    }
    actor_set_blockmap(a,BM_LAYER_HAZARD);
}

void barrel_attack(Actor * a)
{    
    if (!(a->x==player->x && a->y==player->y))
    return;
    SPR_setAnim(a->sprite,2);
    XGM2_playPCM(snd_explode,sizeof(snd_explode),SOUND_PCM_CH2);
    attacker_count=1;
    actor_attacking=NULL;
    actor_defending=player;
    gm_timer=0;
    gm_state=GAME_STATE_ATTACK;
    a->timer=1;
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
    a->collision_layer=BM_LAYER_HAZARD;
    a->collision_mask=BM_MASK_POINTY;
    a->facing_dir=facing_dir;
    a->act_move_start=pointy_update;
    a->act_move_finish=pointy_attack;
    a->sprite = SPR_addSprite(&spr_pointy,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrameChangeCallback(a->sprite, &pointy_animate);
    pointy_animate(a->sprite);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH+2);
    actors_spawned++;
}

void pointy_attack(Actor * a)
{
    if (!(a->x==player->x && a->y==player->y))
    return;
    attacker_count=1;
    actor_attacking=NULL;
    actor_defending=player;
    gm_timer=0;
    gm_state=GAME_STATE_ATTACK;
}

void pointy_update(Actor * a)
{
    int8_t target_x,target_y;
    target_x=a->x+dir_get_x(a->facing_dir);
    target_y=a->y+dir_get_y(a->facing_dir);

    if (blockmap_check(target_x,target_y) & BM_MASK_POINTY)
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

    if (a->target_x==player->x && a->target_y==player->y && a->x==player->target_x && a->y==player->target_y) //Intercept player movement
    {
        player->target_x=player->x;
        player->target_y=player->y;
    }
    actor_set_blockmap(a,BM_LAYER_HAZARD);
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
    a->collision_layer=BM_LAYER_ENEMY;
    a->collision_mask=BM_MASK_BONEYM;
    a->act_move_start=boneym_turn_start;
    a->act_move_finish=boneym_attack;
    a->act_counterattack=boneym_attack;
    a->sprite = SPR_addSprite(&spr_boneym,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    
    SPR_setAutoTileUpload(a->sprite, FALSE);
    actor_face_dir(a);
    SPR_setFrameChangeCallback(a->sprite, &boneym_animate);
    boneym_animate(a->sprite);
    SPR_setDepth(a->sprite,SPR_MIN_DEPTH+2);
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
                actor_set_blockmap(a,BM_LAYER_ENEMY);
                return;
            }
        }

        if (!(blockmap_check(target_x,target_y) & BM_MASK_BONEYM))
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
    actor_set_blockmap(a,BM_LAYER_ENEMY);
}

void boneym_attack(Actor * a)
{
    if (gm_state==GAME_STATE_GATE)
    return;

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