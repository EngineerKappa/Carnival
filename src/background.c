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

	BG_load();
}

void BG_load()
{
	PAL_setPalette(PAL2, canyon_palette.data, DMA);
	//Load BG Tiles
	BG_VRAM_ind=VRAM_ind;
	VDP_loadTileSet(&canyon_bg_tileset, BG_VRAM_ind, DMA);
	VRAM_ind += canyon_bg_tileset.numTile;
	//Load FG Tiles
	FG_VRAM_ind = VRAM_ind;
	VDP_loadTileSet(&canyon_tileset, FG_VRAM_ind, DMA);
	VRAM_ind += canyon_tileset.numTile;
	//Load Tilemaps
	bgb = MAP_create(&canyon_bg_map, BG_B, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, BG_VRAM_ind));
	bga = MAP_create(&canyon_map, BG_A, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, FG_VRAM_ind));
}