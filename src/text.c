#include <headers.h>
/// @brief //////////////////////////////////
char *str_hint_first = "I need to escape CASTLE KRANION! The portal should take me to the next floor.";
char *str_hint_wait = "I can press the A, B, or C buttons if I need to STALL for time!";
char *str_hint_boneym = "I need to hit BONEY Ms from the side! If I hit them from the front, they'll counter-attack!";
char *str_hint_barrel = "Some days you just can't get rid of a barrel... The explosions can still hurt me once it's gone!";

void text_check_room()
{
    textbox_state=TEXT_STATE_TYPING;
    switch (floor_current)
    {
        case 1: p_text_current=str_hint_first; break;
        case 2: p_text_current=str_hint_wait; break;
        case 4: p_text_current=str_hint_boneym; break;
        case 8: p_text_current=str_hint_barrel; break;
    
        default: textbox_state=TEXT_STATE_INACTIVE; break;
    }
}

void text_init()
{
    text_copy_count=0;   
    text_x=TEXTBOX_X;
    text_y=TEXTBOX_Y;
    textbox_anim=0;
    if (gm_current==GM_GAME)
    {
        text_check_room();
    }
    else
    {
        textbox_state=TEXT_STATE_INACTIVE;
    }
}

void textbox_draw_char(char destination_char)
{
    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, TILE_FONT_INDEX-' '+destination_char),text_x,text_y);   
}

void text_update()
{
    if (transition_state!=0)
    return;

    if (textbox_anim<=TEXTBOX_HEIGHT)
    {
        VDP_fillTileMapRect(BG_B,TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, 1),TEXTBOX_X-1,TEXTBOX_Y+TEXTBOX_HEIGHT-textbox_anim-1,TEXTBOX_WIDTH,1);
        textbox_anim++;
        return;
    }
    u8 word_size=0;
    char wrap_char;
    
    char_current=*(p_text_current + text_copy_count);

    for (size_t i = 0; i < 20; i++)
    {
        wrap_char=*(p_text_current + text_copy_count + i);
        
        if (wrap_char!=' ' && wrap_char!='\0')
        {
            word_size++;
        }
        else
        {
            break;
        }
    }
    
    if (text_x+word_size >= (TEXTBOX_X + TEXTBOX_WIDTH - 1))
    {
        text_x=TEXTBOX_X;
        text_y++;
    }
    
    
    switch (char_current)
    {
    case '\n':
        text_y++;
        text_x=TEXTBOX_X;
        break;
    
    case '\0':
        text_copy_count=0;
        text_y++;
        text_y++;
        text_x=0;
        textbox_state=TEXT_STATE_INACTIVE;
        break;
    default:
        textbox_draw_char(char_current);
        text_x++;
        break;
    }
    text_copy_count++;
}