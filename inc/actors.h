
enum {
    OBJ_EMPTY = 0,
    OBJ_PLAYER,
    OBJ_YORB,
    OBJ_EXIT,
    OBJ_BONEYM
};
enum 
{ 
    STATE_NORMAl = 0,
    STATE_MOVE = 0,
};

typedef struct {
    u8 type;
    int x, y;
    int target_x, target_y;
    int scroll_x,scroll_y;
    u8 facing_dir;
    u8 state;
    u16 timer;
    Sprite* sprite;
    bool hflip, vflip;
} Actor;

Actor player;
u8 actors_spawned;
#define MAX_ACTORS 80
Actor actors[MAX_ACTORS];

// animation index table for enemies (static VRAM loading)
u16** sprite_index_yorb;
void yorb_animate(Sprite* sprite);

u8 actor_find_empty_slot();
void actor_free(Actor* a);
void player_init();
void spawn_yorb(int spawn_x,int spawn_y);
void actors_update();
void actors_init();