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
    u8 tile = 0;
    intToStr(floor_current,str,2);
    VDP_drawTextBG(BG_B,str,34,5);
    //HP
    for (size_t i = 0; i < PLAYER_HP_MAX; i++)
    {
        tile = 0;
        if (i >= player_hp )
        {
        tile = 1;
        if (player_hp <= 3)
        tile = 2;
        }
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, UI_VRAM_ind + tile),28+i,9);
    }
    
    //VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, 1),30,9);
    //VDP_drawTextBG(BG_B,"*****",30,9);

    //Yorbs
    intToStr(yorb_count,str,2);
    VDP_drawTextBG(BG_B,str,32,13);

    //Score
    intToStr(score,str,5);
    VDP_drawTextBG(BG_B,str,30,25);
    update_hud=false;
}