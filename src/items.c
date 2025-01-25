#include <headers.h>

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
    yorbs_left++;
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
    if (yorb_count>=50)
    {
        if (player_hp<PLAYER_HP_MAX)
        player_hp++;
        XGM2_playPCM(snd_heart,sizeof(snd_heart),SOUND_PCM_CH3);
        yorb_count=0;
    }

    SPR_setAnim(a->sprite,1);
    a->act_realtime=effect_update;
    XGM2_playPCM(snd_yorb,sizeof(snd_yorb),SOUND_PCM_CH2);
    score+=10;
    update_hud=true;
    yorbs_left--;
    if (yorbs_left==0)
    {
        if (trophy!=NULL)
        {
            SPR_setVisibility(trophy->sprite,VISIBLE);
            XGM2_playPCM(snd_cymbal,sizeof(snd_cymbal),SOUND_PCM_CH3);
        }
    }
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

void spawn_heart(int spawn_x,int spawn_y)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    gate = a;
    a->type = OBJ_HEART;
    a->x = spawn_x;
    a->y = spawn_y;
    
    a->sprite = SPR_addSprite(&spr_heart,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    set_sprite_index(a->sprite,sprite_index_heart);
    actors_spawned++;
}

void heart_collect(Actor * a)
{
    a->type=OBJ_EFFECT;
    actor_free(a);
    if (player_hp<PLAYER_HP_MAX)
    player_hp++;
    XGM2_playPCM(snd_heart,sizeof(snd_heart),SOUND_PCM_CH3);
    update_hud=true;
}

void spawn_trophy(int spawn_x,int spawn_y)
{
    Actor *a = &actors[actor_find_empty_slot()];
    actor_set_defaults(a);
    
    gate = a;
    a->type = OBJ_TROPHY;
    a->x = spawn_x;
    a->y = spawn_y;
    
    a->sprite = SPR_addSprite(&spr_trophy,WINDOW_X+a->x * 16 ,WINDOW_Y+a->y * 16 - 4,TILE_ATTR(PAL0,0,FALSE,a->hflip));
    SPR_setAutoTileUpload(a->sprite, FALSE);
    SPR_setVisibility(a->sprite,HIDDEN);
    set_sprite_index(a->sprite,sprite_index_trophy);
    actors_spawned++;
    trophy=a;
}


void trophy_collect(Actor * a)
{
    if (yorbs_left>0)
    return;

    a->type=OBJ_EFFECT;
    actor_free(a);
    XGM2_playPCM(snd_trophy,sizeof(snd_trophy),SOUND_PCM_CH3);
    score+=700;
    update_hud=true;
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
    actors_spawned++;
}

void gate_animate(Sprite* sprite)
{
    u16 tileIndex = sprite_index_gate[sprite->animInd][sprite->frameInd];
    SPR_setVRAMTileIndex(sprite,tileIndex);
}
