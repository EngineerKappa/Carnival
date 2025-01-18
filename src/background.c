#include <headers.h>

u8 palcycle_index;
u8 palcycle_wait;
bool palcycle_bg_enabled;
u16 palette[64];


void BG_init()
{
	background_x = 0;
	background_y = 0;
	VRAM_ind = TILE_USER_INDEX;
	UI_VRAM_ind = TILE_USER_INDEX;
	BG_VRAM_ind = TILE_USER_INDEX;
	SPR_VRAM_ind = TILE_SPRITE_INDEX;
}

void BG_load_frame()
{
	////Load this before we load any other graphics
	VRAM_ind=TILE_USER_INDEX;
	VDP_loadTileSet(&ui_tileset, VRAM_ind, DMA);
	//bgb = VDP_setTileMap(&ui_map, BG_B, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, VRAM_ind));
	VRAM_ind += ui_tileset.numTile;
	UI_VRAM_ind = VRAM_ind;
	VDP_loadTileSet(&ui_icon_tiles,UI_VRAM_ind,DMA);
	VRAM_ind+=ui_icon_tiles.numTile;
	BG_VRAM_ind = VRAM_ind;
	////Load Tilemaps
	
}

void BG_display_frame()
{
	VDP_setTileMapEx(BG_B,&ui_map,TILE_USER_INDEX,0,0,0,0,40,30,DMA);
	//MAP_scrollTo(bgb, 0,0);
	SYS_doVBlankProcess(); // This feels hacky but we need to update the tilemap before we can draw new stuff to the tilemap
}