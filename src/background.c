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
	VRAM_ind = TILE_USER_INDEX;
	BG_VRAM_ind = 1;
	FG_VRAM_ind = 1;
	BG_load_frame();
	
}

void BG_load_frame()
{
	//Load Frame
	FG_VRAM_ind = VRAM_ind;
	VDP_loadTileSet(&ui_tileset, FG_VRAM_ind, DMA);
	VRAM_ind += ui_tileset.numTile;
	bgb = MAP_create(&ui_map, BG_B, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, FG_VRAM_ind));
	//Load Tilemaps
	
}

void BG_display_frame()
{
	MAP_scrollTo(bgb, 0,0);
}