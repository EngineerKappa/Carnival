#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#define WINDOW_X 16
#define WINDOW_Y 16
#define ROOM_SIZE 11 //Every Room is square, this is both X and Y

u16 VRAM_ind;
u16 BG_VRAM_ind;
u16 FG_VRAM_ind;
s16 background_x;
s16 background_y;

Map* bga;
Map* bgb;

void BG_init();
void BG_load();


#endif // _BACKGROUND_H_