#include <headers.h>

void transition_init()
{
    transition_state=0;
    transition_scroll=0;
}

void transition_start(void *callback)
{
    transition_callback=callback;
    
    transition_scroll=0;
    transition_state=1;
}

void transition_animate()
{
    switch (transition_state) 
	{
		case 1:
			transition_animate_enter();
			break;
        case 2:
			transition_animate_exit();
			break;
	}
}

void transition_animate_enter()
{
    u8 i;
    u8 tile = 11;
    u8 y = 0;
    for(i = 0; i < 40; i++)
    {
        if (i < 10)
        {
            y=transition_scroll;
            tile=11;
        }
        if (i >= 10 && i < 20)
        {
            y=28-transition_scroll;
            tile=14;
        }
        if (i >= 20 && i < 30)
        {
            y=transition_scroll;
            tile=4;
        }
        if (i >= 30)
        {
            y=28-transition_scroll;
            tile=7;
        }
        if (game_clear)
        tile=7;

            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, tile),i,y);
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, tile),i,y+1);
    }
    
    transition_scroll+=2;



    if (transition_scroll>28)
    {
        transition_state=2;
        transition_scroll=0;
        transition_callback();

    }
}

void transition_animate_exit()
{
    u8 i;

    u8 y = 0;
    for(i = 0; i < 40; i++)
    {
        if (i < 10)
        {
            y=transition_scroll;
        }
        if (i >= 10 && i < 20)
        {
            y=28-transition_scroll;
        }
        if (i >= 20 && i < 30)
        {
            y=transition_scroll;
        }
        if (i >= 30)
        {
            y=28-transition_scroll;
        }
            VDP_setTileMapXY(BG_A, 0,i,y);
            VDP_setTileMapXY(BG_A, 0,i,y+1);
    }
    
    i++;
    transition_scroll+=2;

   

    if (transition_scroll>28)
    {
        transition_state=0;
        transition_scroll=0;
        
    }
}



