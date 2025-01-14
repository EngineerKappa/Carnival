#include <headers.h>
void game_draw_hud_labels()
{
    VDP_drawTextBG(BG_B,"Castle",28,2);
    VDP_drawTextBG(BG_B,"  Kranion",28,3);
    VDP_drawTextBG(BG_B,"Swordsman",28,7);
    VDP_drawTextBG(BG_B,"Floor ",28,5);
    VDP_drawTextBG(BG_B,"Yorbs",28,11);
    VDP_drawTextBG(BG_B,"Score",28,23);
}

void game_draw_hud_text()
{
    if (!update_hud)
    return;
    char str[10];
    
    intToStr(floor_current,str,2);
    VDP_drawTextBG(BG_B,str,34,5);

    //HP
    VDP_drawTextBG(BG_B,"*****",30,9);

    //Yorbs
    intToStr(yorb_count,str,3);
    VDP_drawTextBG(BG_B,str,30,13);

    //Score
    intToStr(0,str,3);
    VDP_drawTextBG(BG_B,str,31,25);
    update_hud=false;
}