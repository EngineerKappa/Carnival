#include <headers.h>

void actor_sprite_init()
{
    u16 numTile;
    SPR_VRAM_ind=SPR_getFreeVRAM();
    sprite_index_yorb=SPR_loadAllFrames(&spr_yorb,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_gate=SPR_loadAllFrames(&spr_gate,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_heart=SPR_loadAllFrames(&spr_heart,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_boneym=SPR_loadAllFrames(&spr_boneym,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_pointy=SPR_loadAllFrames(&spr_pointy,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_trophy=SPR_loadAllFrames(&spr_trophy,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_fusedropper=SPR_loadAllFrames(&spr_fusedropper,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_barrel=SPR_loadAllFrames(&spr_barrel,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_sword=SPR_loadAllFrames(&spr_sword,SPR_VRAM_ind,&numTile);
    
}

void actors_init(){
    actors_spawned=0;
    fusedropper_timer=0;
    turn_updated=false;
    actor_sprite_init();
    player_init();
}

inline void actor_set_defaults(Actor *a)
{
    a->type = OBJ_EMPTY;
    a->x = 0;
    a->y = 0;
    a->target_x = 0;
    a->target_y = 0;
    a->scroll_x = 0;
    a->scroll_y = 0;
    a->collision_layer = 0;
    a->collision_mask = 0;
    a->state = 0;
    a->frame = 0;
    a->timer = 0;
    a->hflip = false;
    a->vflip = false;
    a->sprite = NULL;
    a->act_move_start = NULL;
    a->act_move_finish = NULL;
    a->act_realtime = NULL;
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

void actors_clear_all(bool delete_player)
{
    u8 i=1;
    if (delete_player)
    i=0;    
    for(; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY))
        actor_free(a);
    }
}

void actors_update()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY) && (a->act_move_start!=NULL))
        a->act_move_start(a);
    }
    fusedropper_timer++;
    if (fusedropper_timer==3)
    {
        fusedropper_timer=0;
    }
}

void actor_face_dir(Actor* a)
{
    if (a->type == OBJ_FUSEDROPPER) //hack
    return;

    if (a->facing_dir>DIR_DOWN)
    a->facing_dir=DIR_RIGHT;
    switch (a->facing_dir) 
	{
		case DIR_RIGHT:
			SPR_setAnimAndFrame(a->sprite, 1,a->frame);
			SPR_setHFlip(a->sprite,false);
			break;
		case DIR_UP:
			SPR_setAnimAndFrame(a->sprite, 2,a->frame);
			SPR_setHFlip(a->sprite,false);
			break;
		case DIR_DOWN:
			SPR_setAnimAndFrame(a->sprite, 0,a->frame);
			SPR_setHFlip(a->sprite,false);
			break;
		case DIR_LEFT:
			SPR_setAnimAndFrame(a->sprite, 1,a->frame);
			SPR_setHFlip(a->sprite,true);
			break;
	}
}

bool actor_target_forward(Actor* a)
{
    u8 dir=a->facing_dir;
    int8_t target_x,target_y;
    target_x=a->x + dir_get_x(dir);
    target_y=a->y + dir_get_y(dir);

    if (!(blockmap_check(target_x,target_y) & a->collision_mask))
    {
        a->target_x=target_x;
	    a->target_y=target_y;
        return true;
    }
    {
        return false;
    }
}

int8_t dir_get_x(u8 dir)
{
    switch (dir) 
	{
		case DIR_RIGHT:
			return 1;
		case DIR_UP:
			return 0;
		case DIR_DOWN:
			return 0;
		case DIR_LEFT:
			return -1;
		default:
            return 0;
	}
}

int8_t dir_get_y(u8 dir)
{
    switch (dir) 
	{
		case DIR_RIGHT:
			return 0;
		case DIR_UP:
			return -1;
		case DIR_DOWN:
			return 1;
		case DIR_LEFT:
			return 0;
        default:
            return 0;
	}
}

u8 dir_get_180(u8 dir)
{
    switch (dir) 
	{
		case DIR_RIGHT:
			return DIR_LEFT;
		case DIR_UP:
			return DIR_DOWN;
		case DIR_DOWN:
			return DIR_UP;
		case DIR_LEFT:
			return DIR_RIGHT;
        default:
            return 0;
	}
}

void actor_set_position(Actor* a, u8 target_x,u8 target_y)
{
    a->x=target_x;
    a->y=target_y;
    a->target_x=target_x;
    a->target_y=target_y;

    SPR_setPosition(a->sprite,WINDOW_X+(target_x << 4),WINDOW_Y+(target_y << 4) - 4);
}

void game_scroll_all_actors()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_move_start!=NULL))
        actor_scroll(a);
    }
}

void actor_scroll(Actor* a)
{
    s8 x=a->x;
    s8 y=a->y;
    s8 tx=a->target_x;
    s8 ty=a->target_y;

    if (x > tx)
        {a->scroll_x=-game_pixels_scrolled;}
    else if (x < tx)
        {a->scroll_x=game_pixels_scrolled;}
    
    if (y < ty)
        {a->scroll_y=game_pixels_scrolled;}
    else if (y > ty)
        a->scroll_y=-game_pixels_scrolled;
    
    SPR_setPosition(a->sprite,WINDOW_X+(x << 4) + a->scroll_x, WINDOW_Y+(y << 4) - 4 + a->scroll_y);
}

void game_scroll_end()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_move_start!=NULL))
        actor_scroll_finish(a);
    }
}

void game_update_actors_realtime()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_realtime!=NULL))
        a->act_realtime(a);
    }
}

void actor_set_blockmap(Actor * a, u8 collision_layer)
{
    blockmap[a->x + (a->y*13)] |= collision_layer;
    blockmap[a->target_x + (a->target_y*13)] |= collision_layer;
    //place_tile(a->x,a->y,2);
    //place_tile(a->target_x,a->target_y,2);
}

void actor_remove_from_blockmap(Actor * a, u8 collision_layer)
{
    blockmap[a->x + (a->y*13)]&=~collision_layer;
}

void actor_clear_blockmap(Actor * a)
{
    blockmap[a->x + (a->y*13)]=0;
    //place_tile(a->x,a->y,0);
}

void actor_scroll_finish(Actor * a)
{
    actor_remove_from_blockmap(a,BM_MASK_ACTORS);
    a->x=a->target_x;
    a->y=a->target_y;
    a->scroll_x=0;
    a->scroll_y=0;
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16 + a->scroll_x, WINDOW_Y+a->y * 16 - 4 + a->scroll_y);
    if ((a->type != OBJ_EMPTY)  && (a->act_move_finish!=NULL))
    {
        
        a->act_move_finish(a);
    }
}

void set_sprite_index(Sprite* sprite,SpriteIndex sprite_index)
{
    u16 tileIndex = sprite_index[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}

