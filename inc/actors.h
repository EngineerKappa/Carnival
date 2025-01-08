
enum {
    OBJ_EMPTY = 0,
    OBJ_PLAYER,
    OBJ_YORB,
    OBJ_GATE,
    OBJ_BONEYM
};
enum 
{ 
    STATE_NORMAl = 0,
    STATE_MOVE = 0,
};

struct Actor;
typedef void (*FunctionPointer)(struct Actor*);

#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_LEFT 3
#define DIR_DOWN 4

typedef struct Actor {
    u8 type;
    int x, y;
    int target_x, target_y;
    int scroll_x,scroll_y;
    u8 facing_dir;
    u8 state;
    u16 timer;
    Sprite* sprite;
    bool hflip, vflip;
    u8 frame;
    FunctionPointer act_move_start;
    FunctionPointer act_move_finish;
} Actor;

Actor *player;
u8 actors_spawned;
#define MAX_ACTORS 80
Actor actors[MAX_ACTORS];

// animation index table for enemies (static VRAM loading)
u16** sprite_index_yorb;
u16** sprite_index_gate;


u8 actor_find_empty_slot();
void actor_free(Actor* a);

void actors_clear_all();

void spawn_yorb(int spawn_x,int spawn_y);
void spawn_gate(int spawn_x,int spawn_y);

void yorb_animate(Sprite* sprite);
void gate_animate(Sprite* sprite);

void actors_update();
void actors_init();

void actor_set_position(Actor* a, u8 target_x,u8 target_y);
void game_move_actors();
void actor_move(Actor* a);
void game_move_end();
void actor_move_finish(Actor * a);
void actor_turn(Actor* a);
void actors_update();

bool turn_updated;