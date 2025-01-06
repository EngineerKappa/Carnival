#include <genesis.h>
#include <gfx.h>
#include <actors.h>
#include <background.h>
#include <player.h>

void actor_sprite_init()
{
    sprite_index_yorb=SPR_loadAllFrames(&spr_yorb,TILE_SPRITE_INDEX+32,NULL);
}

void actors_init(){
    actors_spawned=0;
    turn_updated=false;
    actor_sprite_init();
    player_init();
}

void actor_set_defaults(Actor *a)
{
    a->type = OBJ_EMPTY;
    a->x = 0;
    a->y = 0;
    a->target_x = 0;
    a->target_y = 0;
    a->scroll_x = 0;
    a->scroll_y = 0;
    a->state = 0;
    a->frame = 0;
    a->timer = 0;
    a->hflip = false;
    a->vflip = false;
    a->sprite = NULL;
    a->act_step_start = NULL;
}

u8 actor_find_empty_slot()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if (a->type == OBJ_EMPTY)
        {
            return (i);
        }
    }
    return (MAX_ACTORS-1);
}

void actor_free(Actor* a)
{
    SPR_releaseSprite(a->sprite);
    actor_set_defaults(a);
    actors_spawned--;
}

void actors_update()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY) && (a->act_step_start!=NULL))
        a->act_step_start(a);
    }
}

void actor_turn(Actor* a)
{
	a->target_x=a->x;
	a->target_y=a->y;

	switch (a->facing_dir) 
	{
		case DIR_RIGHT:
			a->target_x=a->x+1;
			SPR_setAnimAndFrame(player->sprite, 1, player->frame);
			SPR_setHFlip(player->sprite,false);
			break;
		case DIR_UP:
			a->target_y=a->y-1;
			SPR_setAnimAndFrame(a->sprite, 2, a->frame);
			SPR_setHFlip(a->sprite,false);
			break;
		case DIR_DOWN:
			a->target_y=a->y+1;
			SPR_setAnimAndFrame(a->sprite, 0, a->frame);
			SPR_setHFlip(a->sprite,false);
			break;
		case DIR_LEFT:
			a->target_x=a->x-1;
			SPR_setAnimAndFrame(a->sprite, 1, a->frame);
			SPR_setHFlip(a->sprite,true);
			break;
	}
}

void actor_move(Actor* a)
{
    a->x=a->target_x;
    a->y=a->target_y;
    a->x=clamp(a->x,0,ROOM_SIZE);
	a->y=clamp(a->y,0,ROOM_SIZE);
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16,WINDOW_Y+a->y * 16);
}

void spawn_yorb(int spawn_x,int spawn_y)
{

    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    a->type = OBJ_YORB;
    a->x = spawn_x;
    a->y = spawn_y;

    a->sprite = SPR_addSprite(&spr_yorb,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    s16 frame = (((a->x + a->y)) % 4);
    SPR_setFrame(a->sprite,frame);
    SPR_setFrameChangeCallback(a->sprite, &yorb_animate);
    yorb_animate(a->sprite);
    actors_spawned++;
}
    
void yorb_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_yorb[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}





