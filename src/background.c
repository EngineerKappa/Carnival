#include <genesis.h>
#include <background.h>
#include <gfx.h>

u8 palcycle_index;
u8 palcycle_wait;
bool palcycle_bg_enabled;
u16 palette[64];

void BG_init()
{
	background_x = 0;
	background_y = 0;
	VRAM_ind = 1;
	BG_VRAM_ind = 1;
	portrait_VRAM_ind = 1;
	PAL_setColors(PAL1*16,palette_black,16,DMA);
	memset(scroll_data_b, 0, sizeof(scroll_data_b));
	VDP_setScrollingMode(HSCROLL_TILE,VSCROLL_PLANE);
}


void loadBackground(Image image)
{
	PAL_fadeInPalette(1,image.palette->data,30,TRUE);
	VDP_drawImageEx(BG_B, &image, TILE_ATTR_FULL(PAL1, false, false, false, BG_VRAM_ind),0,0,false,TRUE);
	VRAM_ind = BG_VRAM_ind+image.tileset->numTile;
	portrait_VRAM_ind=VRAM_ind;
	waitMs(800);
}