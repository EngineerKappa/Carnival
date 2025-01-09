
typedef u8 room[169];
extern room room_data;



void room_init();
void room_end();
void place_tile(u8 x, u8 y,u8 start_tile);
void place_floor(u8 x, u8 y);

enum {
    TD_EMPTY = 0,
    TD_SOLID,
};  

extern const u8 tiledefs[16];

bool tile_check_wall(u8 x, u8 y);


enum {
    RS_PLAYER = 128,
    RS_YORB,
    RS_GATE,
    RS_HEART,
    RS_TROPHY,
    RS_BONEYM_UP,
    RS_BONEYM_DOWN,
    RS_BONEYM_LEFT,
    RS_BONEYM_RIGHT
};