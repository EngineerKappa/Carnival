#include <headers.h>

void menu_init()
{
    menu_state=MENU_STATE_INACTIVE;
    menu_option=0;
    menu_x=0;
    menu_y=0;
    menu_size=0;
    menu_option_previous=0;
}

void menu_create(u8 x, u8 y)
{
    menu_state=MENU_STATE_ACTIVE;
    menu_option=menu_option_previous;
    menu_x=x;
    menu_y=y;
    menu_timer=0;
    menu_size=0;
}

void menu_add_option(const char *label, FunctionPointer menu_action)
{
    strcpy(menu_list[menu_size].label,label);
    menu_list[menu_size].menu_action=menu_action;
    menu_size++;
}

void menu_update()
{
    
    bool update=false;
    if (menu_state==MENU_STATE_ACTIVE)
    {
        if (joy_pressed(BUTTON_START))
        {
            XGM2_playPCM(snd_ding,sizeof(snd_ding),SOUND_PCM_CH2);
            menu_state=MENU_STATE_PRESSED;
            menu_timer=0;
            menu_list[menu_option].menu_action();
        }

        if (joy_pressed(BUTTON_UP))
        {
            if (menu_option==0)
            {
                menu_option=menu_size-1;
            }
            else
            {
                menu_option--;
            }
            update=true;
        }

        if (joy_pressed(BUTTON_DOWN))
        {
            menu_option++;
            update=true;
        }
        

        if (update)
        {
            menu_timer=0;
            if (menu_option>=menu_size)
            {
                menu_option=0;
            }

            menu_draw(false);
        }
    }
    
    if (menu_timer == 1)
    {
        menu_draw(false);
    }

    if (menu_timer == 20)
    {
        menu_draw(true);
    }

    if (menu_timer == 30)
    {
        menu_timer=0;
    }

    menu_timer++;
}

void menu_clear()
{
    VDP_clearTextArea(menu_x-2,menu_y,MENU_TEXT_SIZE + 4,menu_size*2);
}

void menu_draw(bool flash)
{
    
    for (size_t i = 0; i < menu_size; i++)
    {
        u8 x = menu_x;
        u8 y = menu_y+(i*2);
        if ((menu_option==i && !flash) | (i!=menu_option))
        {
            VDP_drawTextBG(BG_A,menu_list[i].label,x,y);
        }
        else
        {
            VDP_clearTextBG(BG_A,x,y,MENU_TEXT_SIZE);
        }

        if (menu_option==i)
        {
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, UI_VRAM_ind + 3 + flash),x-2,y);
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, TRUE, UI_VRAM_ind + 3 + flash),x+MENU_TEXT_SIZE+1,y);
        }
        else
        {
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, 0),x-2,y);
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, TRUE, 0),x+MENU_TEXT_SIZE+1,y);
        }

    }
    
}
