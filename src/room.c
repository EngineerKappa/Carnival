#include <genesis.h>
#include <gfx.h>
#include <system.h>
#include <actors.h>
#include <player.h>
#include <background.h>
#include <room.h>


void room_init()
{
    u8 x,y,i;
    x=0;
    y=0;

    for (i=0; i<169; i++)
    {
        switch (room_data[i]) 
        {
            case 1:
                player->target_x=x;
                player->target_y=y;
                actor_move(player);
                break;
            case 2:
                place_tile(x,y,4);
                break;
            case 3:
                place_tile(x,y,8);
                spawn_yorb(x,y);
                break;
        }
        

        x++;
        if (x>ROOM_SIZE)
        {
            x=0;
            y++;
        }
    }
}

void place_tile(u8 x,u8 y,u8 start_tile)
{
    VDP_setTileMapXY(BG_B, start_tile,1+x*2,1+y*2);
    VDP_setTileMapXY(BG_B, start_tile+1,1+x*2+1,1+y*2);
    VDP_setTileMapXY(BG_B, start_tile+2,1+x*2,1+y*2+1);
    VDP_setTileMapXY(BG_B, start_tile+3,1+x*2+1,1+y*2+1);
}