#include <genesis.h>
#include <gfx.h>
#include <background.h>
#include <actors.h>
#include <player.h>
#include <transition.h>


void player_init(){
    //The player should be spawned first
    Actor *pl = &actors[0];
    pl->x = 0;
    pl->y = 0;
    pl->type = OBJ_PLAYER;
    pl->state=0;
    pl->frame=0;
    pl->hflip = false;
    pl->vflip = false;
    pl->sprite = SPR_addSprite(&spr_swordsman,WINDOW_X+pl->x * 16 ,WINDOW_Y+pl->y * 16,TILE_ATTR(PAL1,0,FALSE,pl->hflip));
    pl->act_move_start = player_step;
    pl->act_move_finish = player_collect_item;
    actors_spawned=1;
    player = pl;
}

void player_step()
{
    
    //player_collect_item();
}

void player_collect_item()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        
        Actor* a = &actors[i];
        if ( a->x == player->x && a->y == player->y)
        {
            switch (a->type)
            {
            case OBJ_YORB:
                actor_free(a);
                break;
            case OBJ_GATE:
                transition_start();
                break;
            }
            
        }
    }
}