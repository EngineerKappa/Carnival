
#define ROOM_MAX_TILES 169
typedef u8 room[ROOM_MAX_TILES];
room room_data;
bool blockmap[ROOM_MAX_TILES];
room* room_list[50];
u8 MAX_ROOMS;

extern room floor_1;
extern room floor_2;
extern room floor_3;
extern room floor_4;
extern room floor_5;
extern room floor_6;
extern room floor_7;
extern room floor_8;
extern room floor_9;
extern room floor_10;
extern room floor_11;
extern room floor_12;
extern room floor_14;
extern room floor_15;
extern room floor_16;
extern room floor_17;
extern room floor_18;
extern room floor_19;
extern room floor_20;


void room_list_init();
void room_init();
void room_load();
void room_end(bool restart);
void place_tile(u8 x, u8 y,u8 start_tile);
void place_floor(u8 x, u8 y);

enum {
    TD_EMPTY = 0,
    TD_SOLID,
};  

extern const u8 tiledefs[16];

bool tile_check_wall(u8 x, u8 y, bool check_actors);


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