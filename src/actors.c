#include <genesis.h>
#include <gfx.h>
#include <actors.h>
#include <background.h>

void actor_sprite_init()
{
    sprite_index_yorb=SPR_loadAllFrames(&spr_yorb,TILE_SPRITE_INDEX+32,NULL);
}

void player_collect_item()
{
    u8 i;
    for(i = 0; i < MAX_ACTORS; i++)
    {
        
        Actor* a = &actors[i];
        if ((a->type == OBJ_YORB) && a->x == player->x && a->y == player->y)
        {
            actor_free(a);
        }
    }
}

void actors_init(){
    actors_spawned=0;
    turn_updated=false;
    actor_sprite_init();
    
    player_init();

    int spawn_x = 2;
    int spawn_y = 6;
    u8 i;
    for(i = 0; actors_spawned < MAX_ACTORS; i++)
    {
            spawn_yorb(spawn_x,spawn_y);
            spawn_x ++;
            if (spawn_x > 16)
            {
                spawn_x = 2;
                spawn_y ++;

            }
    }
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
    a->timer = 0;
    a->hflip = false;
    a->vflip = false;
    a->sprite = NULL;
    a->act_step_start = NULL;
}

void player_init(){
    //The player should be spawned first
    Actor *pl = &actors[0];
    pl->x = 4;
    pl->y = 4;
    pl->type = OBJ_PLAYER;
    pl->state=0;
    pl->hflip = false;
    pl->vflip = false;
    pl->sprite = SPR_addSprite(&spr_swordsman,pl->x * 16 ,pl->y * 16,TILE_ATTR(PAL1,0,FALSE,pl->hflip));
    pl->act_step_start = &actor_step_test;
    actors_spawned=1;
    player = pl;
}

void spawn_yorb(int spawn_x,int spawn_y)
{

    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    a->type = OBJ_YORB;
    a->x = spawn_x;
    a->y = spawn_y;

    a->sprite = SPR_addSprite(&spr_yorb,a->x * 16 ,a->y * 16,TILE_ATTR(PAL0,0,FALSE,a->hflip));
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

void actor_free(Actor* a){
    SPR_releaseSprite(a->sprite);
    actor_set_defaults(a);
    actors_spawned--;
    
}

void actor_step_test(Actor* a)
{
    a->x=a->target_x;
    a->y=a->target_y;
    SPR_setPosition(a->sprite,a->x * 16,a->y * 16);
    player_collect_item();
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