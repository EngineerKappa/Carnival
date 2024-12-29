#include <genesis.h>
#include <gfx.h>
#include <actors.h>

void actors_init(){
    actors_spawned=0;
    player_init();
    spawn_yorb(2,2);
    spawn_yorb(2,3);
    spawn_yorb(2,4);
    spawn_yorb(2,5);
}

void player_init(){
    //The player should be spawned first
    Actor *pl = &actors[0];
    pl->x = 4;
    pl->y = 4;
    pl->type = OBJ_PLAYER;
    pl->state=0;
    pl->sprite = SPR_addSprite(&spr_swordsman,pl->x * 16 ,pl->y * 16,TILE_ATTR(PAL1,0,FALSE,FALSE));
    actors_spawned=0;
}

void spawn_yorb(int spawn_x,int spawn_y)
{
    Actor *a = &actors[actors_spawned];
    a->type = OBJ_YORB;
    a->x = spawn_x;
    a->y = spawn_y;
    a->state = 0;
    a->timer = 0;
    a->sprite = SPR_addSprite(&spr_yorb,a->x * 16 ,a->y * 16,TILE_ATTR(PAL0,0,FALSE,FALSE));
    actors_spawned++;
}

u8 actor_find_empty_slot()
{
    Actor* e = actors;
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        if (e->type == OBJ_EMPTY)
        {
            return (i);
        }
    }
    return (MAX_ACTORS-1);
}

void actor_free(Actor* a){
    a->type = OBJ_EMPTY;
	a->state = 0;
    a->timer = 0;
	SPR_setVisibility(a->sprite,HIDDEN);
}
