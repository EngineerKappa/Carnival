#include <genesis.h>
#include <gfx.h>
#include <system.h>
#include <actors.h>
#include <player.h>
#include <background.h>
#include <gm_game.h>
#include <room.h>

const u8 tiledefs[16] = { 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void room_init()
{
    u8 x,y,i;
    x=0;
    y=0;
    game_pixels_scrolled=0;
    game_draw_hud_text();
    VDP_setHilightShadow(false);

    for (i=0; i<169; i++)
    {
        switch (room_data[i]) 
        {
            case RS_PLAYER:
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
}

void place_floor(u8 x, u8 y)
{
    u8 tile = 0;
    if ((x==ROOM_SIZE) | (tile_check_wall(x + 1, y)))
    tile=1;
    place_tile(x,y,tile);
}

bool tile_check_wall(u8 x, u8 y)
{
    u8 tile=room_data[x + (y*13)];

    if (tiledefs[tile]==TD_SOLID && tile < RS_PLAYER) 
    {
        return true;
    }
    return false;
}

void place_tile(u8 x,u8 y,u8 start_tile)
{
    start_tile*=4;
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind+start_tile),  1+x*2,1+y*2);
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind+start_tile+1), 1+x*2+1,1+y*2);
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind+start_tile+2), 1+x*2,1+y*2+1);
    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind+start_tile+3),1+x*2+1,1+y*2+1);
}