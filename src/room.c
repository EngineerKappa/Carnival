#include <headers.h>

const u8 tiledefs[16] = {
    0, //Floor
    0, 
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,//Blast Floor
    0,//Blast Floor Shadow
    0,
    0,
    0,
    0,
    0};

void room_set_step_bonus()
{
    switch (floor_current)
    {
    case 1: step_bonus=500; break;
    case 2: step_bonus=600; break;
    case 3: step_bonus=800; break;
    case 6: step_bonus=1400; break;
    case 7: step_bonus=1200; break;
    default:step_bonus=STEP_BONUS_MAX;break;
    }
}

void room_init()
{
    
    gm_state=GAME_STATE_NORMAL;
    gm_timer=0;
    trophy=NULL;
    BG_display_frame();
    game_pixels_scrolled=0;
    update_hud=true;
    yorbs_left=0;
    floor_trophy_found=false;
    attacker_count=0;
    actor_defending_shake=0;
    fusedropper_timer=0;
    floor_trophy_spawned=false;
    room_set_step_bonus();
    u8 x,y,i;
    x=0;
    y=0;
    text_init();
    VDP_waitVBlank(true);
    for (i=0; i<ROOM_MAX_TILES; i++)
    {
        switch (room_data[i]) 
        {
            case RS_PLAYER:
                player_reset();
                actor_set_position(player, x,y);
                place_floor(x,y);
                break;
            case RS_YORB:
                place_floor(x,y);
                spawn_yorb(x,y);
                break;
            case RS_GATE:
                place_floor(x,y);
                spawn_gate(x,y);
                break;
            case RS_HEART:
                place_floor(x,y);
                spawn_heart(x,y);
                break;
            case RS_TROPHY:
                place_floor(x,y);
                spawn_trophy(x,y);
                break;
            case RS_BONEYM_DOWN:
                place_floor(x,y);
                spawn_boneym(x,y,DIR_DOWN);
                break;
            case RS_BONEYM_RIGHT:
                place_floor(x,y);
                spawn_boneym(x,y,DIR_RIGHT);
                break;
            case RS_BONEYM_LEFT:
                place_floor(x,y);
                spawn_boneym(x,y,DIR_LEFT);
                break;
            case RS_BONEYM_UP:
                place_floor(x,y);
                spawn_boneym(x,y,DIR_UP);
                break;
            case RS_POINTY_UP:
                place_floor(x,y);
                spawn_pointy(x,y,DIR_UP);
                break;
            case RS_POINTY_RIGHT:
                place_floor(x,y);
                spawn_pointy(x,y,DIR_RIGHT);
                break;
            case RS_POINTY_LEFT:
                place_floor(x,y);
                spawn_pointy(x,y,DIR_LEFT);
                break;
            case RS_POINTY_DOWN:
                place_floor(x,y);
                spawn_pointy(x,y,DIR_DOWN);
                break;
            
            case RS_FUSEDROPPER_UP:
                place_floor(x,y);
                spawn_fusedropper(x,y,DIR_UP);
                break;
            case RS_FUSEDROPPER_RIGHT:
                place_floor(x,y);
                spawn_fusedropper(x,y,DIR_RIGHT);
                break;
            case RS_FUSEDROPPER_LEFT:
                place_floor(x,y);
                spawn_fusedropper(x,y,DIR_LEFT);
                break;
            case RS_FUSEDROPPER_DOWN:
                place_floor(x,y);
                spawn_fusedropper(x,y,DIR_DOWN);
                break;
            default:
                place_tile(x,y,room_data[i]);
        }
    
        x++;
        if (x>ROOM_SIZE)
        {
            x=0;
            y++;
        }
    }
    game_draw_hud_labels();
    game_draw_hud_text();
}



void room_load()
{
    int n = sizeof(room_data) / sizeof(room_data[0]);
    memcpy(room_data,room_list[floor_current],n);
}

void room_end(bool restart)
{
    SPR_defragVRAM();
    VDP_setHilightShadow(false);
    actors_clear_all(false);

    if (!restart)
    {
        floors_cleared = max(floor_current,floors_cleared);
        sram_save();
        floor_current ++;
        if (floor_current>MAX_ROOMS)
        floor_current=1;
    }
    room_load();
}

void transition_room_next()
{
    room_end(false);
    room_init();
}

inline void place_floor(u8 x, u8 y)
{
    place_tile(x,y,0+(tiledefs[room_data[x+1 + (y*13)]]==TD_SOLID));
}

u8 blockmap_check(u8 x, u8 y)
{
    return blockmap[x + (y*13)];
}

void place_tile(u8 x,u8 y,u8 start_tile)
{
    if (tiledefs[start_tile]==TD_SOLID)
    blockmap[x + (y*13)]=BM_LAYER_WALL;
    else
    blockmap[x + (y*13)]=0;

    start_tile*=4;
    VDP_fillTileMapRectInc(BG_B,TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind+start_tile),  1+x*2,1+y*2,2,2);
}