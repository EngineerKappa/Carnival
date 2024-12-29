
enum {
    OBJ_EMPTY = 0,
    OBJ_PLAYER,
    OBJ_YORB,
    OBJ_EXIT,
    OBJ_BONEYM
};



typedef struct {
    u8 type;
    int x, y;
    int xscroll,yscroll;
    u8 facing_dir;
    u8 state;
    u16 timer;
    Sprite* sprite;
} Actor;

Actor player;
u8 actors_spawned;
#define MAX_ACTORS 8
Actor actors[MAX_ACTORS];

u8 actor_find_empty_slot();
void actor_free(Actor* a);
void player_init();
void spawn_yorb(int spawn_x,int spawn_y);
void actors_update();
void actors_init();