#include <headers.h>

void spawn_boneym(int spawn_x,int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);

    a->type = OBJ_BONEYM;
    a->x = spawn_x;
    a->y = spawn_y;
    a->target_x=spawn_x;
    a->target_y=spawn_y;
    a->facing_dir=facing_dir;
    a->act_move_start=boneym_move;
    a->sprite = SPR_addSprite(&spr_boneym,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    actor_face_dir(a);
    SPR_setFrameChangeCallback(a->sprite, &boneym_animate);
    boneym_animate(a->sprite);
    
    actors_spawned++;
}

void boneym_move(Actor * a)
{
    u16 min_dist=1000;
    int8_t target_x,target_y;

    int8_t px = player->x;
    int8_t py = player->y;
    int8_t ax = a->x;
    int8_t ay = a->y;
    u8 new_dir = 5;
    bool canmove=false;
    u16 dir_dist[4];

   // target_dist=((ax+1-px)*(ax+1-px))+((ay-py)*(ay-py));   

    u8 dir_check=1;
    for (u8 i = 1; i < 8; i++) //Loop through each direction multiple times so we can also compare the last few dirs with the first few ones
    {
        if (dir_check==5)
            dir_check=1;
        target_x=ax+dir_get_x(dir_check);
        target_y=ay+dir_get_y(dir_check);
        if (!tile_check_wall(target_x,target_y,true))
        {
            canmove=true;
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
        }
        else
        {
            if (!actor_move_forward(a))
            {
                a->facing_dir++;
                if (a->facing_dir==5)
                a->facing_dir=1;
            }
        }
    }
    else
    {
        if (!actor_move_forward(a))
        {
            a->facing_dir++;
            if (a->facing_dir==5)
            a->facing_dir=1;
            
        }
        
    }
    actor_face_dir(a);    
}

void boneym_attack(Actor * a)
{
    
}

void boneym_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_boneym[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}