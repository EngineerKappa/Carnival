
enum {
    OBJ_EMPTY = 0,
    OBJ_PLAYER,
    OBJ_YORB,
    OBJ_GATE,
    OBJ_BONEYM,

    OBJ_EFFECT,
};
enum 
{ 
    STATE_NORMAl = 0,
    STATE_MOVE = 0,
};

struct Actor;
typedef void (*FunctionPointer)(struct Actor*);

#define DIR_NONE 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_LEFT 3
#define DIR_DOWN 4



typedef struct Actor {
    FunctionPointer act_realtime;
    FunctionPointer act_move_start;
    FunctionPointer act_move_finish;
    FunctionPointer act_counterattack;
    Sprite* sprite;
    int8_t x, y;
    int8_t target_x, target_y;
    int16_t scroll_x,scroll_y; 
    u8 hp;
    u16 timer;
    u8 facing_dir;
    u8 state;
    u8 frame;
    u8 type;
    bool hflip, vflip;
    
    
} Actor;

Actor *player;
Actor *gate;

Actor *actor_defending;
Actor *actor_attacking;
Actor* attacker_list[4];
u16 actor_defending_palette;
int8_t actor_defending_shake;
u8 attacker_count;
bool actor_defending_will_counter;
u8 actors_spawned;
#define MAX_ACTORS 80
Actor actors[MAX_ACTORS];

// animation index table for enemies (static VRAM loading)
u16** sprite_index_yorb;
u16** sprite_index_gate;
u16** sprite_index_boneym;
u16** sprite_index_pointy;



void actor_set_blockmap(Actor * a);
void actor_clear_blockmap(Actor * a);

u8 actor_find_empty_slot();
void actor_free(Actor* a);

void actors_clear_all(bool delete_player);

void spawn_yorb(int spawn_x,int spawn_y);
void spawn_gate(int spawn_x,int spawn_y);


void yorb_animate(Sprite* sprite);
void gate_animate(Sprite* sprite);

void boneym_turn_start(Actor * a);
void boneym_move(Actor * a);
void boneym_attack(Actor * a);
void pointy_animate(Sprite* sprite);

void yorb_collect(Actor * a);

void effect_run(Actor * a);

void actors_update();
void actors_init();

void actor_set_defaults(Actor *a);
void actor_set_position(Actor* a, u8 target_x,u8 target_y);
void game_move_actors();
void game_run_actors_realtime();
void actor_move(Actor* a);
void game_move_end();
void actor_move_finish(Actor * a);
void actor_face_dir(Actor* a);
bool actor_move_forward(Actor* a);
void actors_update();
int8_t dir_get_x(u8 dir);
int8_t dir_get_y(u8 dir);
u8 dir_get_180(u8 dir);
void enemy_take_damage(Actor * a);

void spawn_boneym(int spawn_x,int spawn_y,u8 facing_dir);
void spawn_pointy(int spawn_x,int spawn_y,u8 facing_dir);
void boneym_animate(Sprite* sprite);

bool turn_updated;


