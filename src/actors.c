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

    if (tile_check_wall(a->target_x,a->target_y))
    {
        a->target_x=a->x;
	    a->target_y=a->y;
    }

}

void actor_set_position(Actor* a, u8 target_x,u8 target_y)
{
    a->x=target_x;
    a->y=target_y;
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

void game_run_actors_realtime()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        Actor* a = &actors[i];
        if ((a->type != OBJ_EMPTY)  && (a->act_realtime!=NULL))
        a->act_realtime(a);
    }
}

void actor_move_finish(Actor * a)
{
    a->x=a->target_x;
    a->y=a->target_y;
    a->scroll_x=0;
    a->scroll_y=0;
    SPR_setPosition(a->sprite,WINDOW_X+a->x * 16 + a->scroll_x, WINDOW_Y+a->y * 16 - 4 + a->scroll_y);
    if ((a->type != OBJ_EMPTY)  && (a->act_move_finish!=NULL))
        a->act_move_finish(a);
}

void spawn_boneym(int spawn_x,int spawn_y,u8 facing_dir)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);

    a->type = OBJ_YORB;
    a->x = spawn_x;
    a->y = spawn_y;

    a->sprite = SPR_addSprite(&spr_boneym,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setFrame(a->sprite,0);
    SPR_setFrameChangeCallback(a->sprite, &boneym_animate);
    boneym_animate(a->sprite);
    actors_spawned++;
}

void boneym_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_boneym[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
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
    game_draw_hud_text();
    SPR_setAnim(a->sprite,1);
    a->act_realtime=effect_run;
    XGM2_playPCM(snd_yorb,sizeof(snd_yorb),SOUND_PCM_CH2);
    
}

void effect_run(Actor * a)
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



