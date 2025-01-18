#include <headers.h>

void actor_sprite_init()
{
    u16 numTile;
    SPR_VRAM_ind=SPR_getFreeVRAM();
    sprite_index_yorb=SPR_loadAllFrames(&spr_yorb,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_gate=SPR_loadAllFrames(&spr_gate,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_boneym=SPR_loadAllFrames(&spr_boneym,SPR_VRAM_ind,&numTile);
    SPR_VRAM_ind+=numTile;
    sprite_index_pointy=SPR_loadAllFrames(&spr_pointy,SPR_VRAM_ind,&numTile);
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
}

void actor_face_dir(Actor* a)
{
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

bool actor_move_forward(Actor* a)
{
    u8 dir=a->facing_dir;
    int8_t target_x,target_y;
    target_x=a->x + dir_get_x(dir);
    target_y=a->y + dir_get_y(dir);

    if (!tile_check_wall(target_x,target_y,true))
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
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16,WINDOW_Y+a->y * 16 - 4);
}

void game_move_actors()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_move_start!=NULL))
        actor_move(a);
    }
}


void actor_move(Actor* a)
{
    if (a->x > a->target_x)
        a->scroll_x=-game_pixels_scrolled;
    if (a->x < a->target_x)
        a->scroll_x=game_pixels_scrolled;
    if (a->y < a->target_y)
        a->scroll_y=game_pixels_scrolled;
    if (a->y > a->target_y)
        a->scroll_y=-game_pixels_scrolled;
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16 + a->scroll_x, WINDOW_Y+a->y * 16 - 4 + a->scroll_y);
}

void game_move_end()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_move_start!=NULL))
        actor_move_finish(a);
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

void actor_set_blockmap(Actor * a)
{
    blockmap[a->x + (a->y*13    )]=true;
    blockmap[a->target_x + (a->target_y*13)]=true;
    //place_tile(a->x,a->y,2);
    //place_tile(a->target_x,a->target_y,2);

}
void actor_clear_blockmap(Actor * a)
{
    blockmap[a->x + (a->y*13)]=false;
    //place_tile(a->x,a->y,0);
}

void actor_move_finish(Actor * a)
{
    actor_clear_blockmap(a);
    a->x=a->target_x;
    a->y=a->target_y;
    a->scroll_x=0;
    a->scroll_y=0;
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16 + a->scroll_x, WINDOW_Y+a->y * 16 - 4 + a->scroll_y);
    actor_set_blockmap(a);
    if ((a->type != OBJ_EMPTY)  && (a->act_move_finish!=NULL))
    {
        
        a->act_move_finish(a);
    }
}

void spawn_yorb(int spawn_x,int spawn_y)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    a->type = OBJ_YORB;
    a->x = spawn_x;
    a->y = spawn_y;

    a->sprite = SPR_addSprite(&spr_yorb,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    s16 frame = (((a->x + a->y)) % 4);
    SPR_setFrame(a->sprite,frame);
    SPR_setFrameChangeCallback(a->sprite, &yorb_animate);
    SPR_setDepth(a->sprite,SPR_MAX_DEPTH);
    yorb_animate(a->sprite);
    actors_spawned++;
}
    
void yorb_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_yorb[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}

void yorb_collect(Actor * a)
{
    a->type=OBJ_EFFECT;
    yorb_count++;
    SPR_setAnim(a->sprite,1);
    a->act_realtime=effect_update;
    XGM2_playPCM(snd_yorb,sizeof(snd_yorb),SOUND_PCM_CH2);
    update_hud=true;
    
}

void effect_update(Actor * a)
{
    a->timer++;
    a->frame++;
    if (a->frame == 5)
    {
        a->hflip=!a->hflip;
        SPR_setHFlip(a->sprite,a->hflip);
    }
    if (a->frame == 10)
    {
        a->vflip=!a->vflip;
        SPR_setVFlip(a->sprite,a->vflip);
        a->frame = 0;
    }
    if (a->timer == 20)
    {
        actor_free(a);
    }
}

void spawn_gate(int spawn_x,int spawn_y)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    gate = a;
    a->type = OBJ_GATE;
    a->x = spawn_x;
    a->y = spawn_y;
    
    a->sprite = SPR_addSprite(&spr_gate,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16,TILE_ATTR(PAL1,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrameChangeCallback(a->sprite, &gate_animate);
    yorb_animate(a->sprite);
    actors_spawned++;
}

void gate_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_gate[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}



