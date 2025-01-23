
enum {
    OBJ_EMPTY = 0,
    OBJ_PLAYER,
    OBJ_YORB,
    OBJ_GATE,
    OBJ_BONEYM,
    OBJ_HEART,
    OBJ_EFFECT,
    OBJ_POINTY,
};
enum 
{ 
    STATE_NORMAl = 0,
    STATE_MOVE,
};

#define DIR_NONE 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_LEFT 3
#define DIR_DOWN 4
#define DIR_NULL 255

struct Actor;
typedef void (*FunctionPointer)(struct Actor*);

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
u8 actors_spawned;
bool actor_defending_will_counter;

#define MAX_ACTORS 80
Actor actors[MAX_ACTORS];

bool turn_updated;

// animation index table for enemies (static VRAM loading)
typedef u16** SpriteIndex;

SpriteIndex sprite_index_yorb;
SpriteIndex sprite_index_gate;
SpriteIndex sprite_index_boneym;
SpriteIndex sprite_index_pointy;
SpriteIndex sprite_index_heart;


//Actor Spawn Management
void actors_init();
u8 actor_find_empty_slot();
void actor_free(Actor* a);
void actor_set_defaults(Actor *a);
void actors_clear_all(bool delete_player);

//Actor Collisions
void actor_set_blockmap(Actor * a);
void actor_clear_blockmap(Actor * a);

//Actor Logic
void actor_set_position(Actor* a, u8 target_x,u8 target_y);
void set_sprite_index(Sprite* sprite,SpriteIndex sprite_index);
void actor_face_dir(Actor* a);
bool actor_target_forward(Actor* a);
int8_t dir_get_x(u8 dir);
int8_t dir_get_y(u8 dir);
u8 dir_get_180(u8 dir);

//Turn Functions
void actors_update();
void game_scroll_all_actors();
void actor_scroll(Actor* a);
void game_scroll_end();
void actor_scroll_finish(Actor * a);
void enemy_take_damage(Actor * a);
void game_update_actors_realtime();

//Yorbs
void spawn_yorb(int spawn_x,int spawn_y);
void yorb_animate(Sprite* sprite);
void yorb_collect(Actor * a);
void effect_update(Actor * a);

//Heart
void spawn_heart(int spawn_x,int spawn_y);
void heart_collect(Actor * a);

//Gate
void spawn_gate(int spawn_x,int spawn_y);
void gate_animate(Sprite* sprite);

//Boney M
void spawn_boneym(int spawn_x,int spawn_y,u8 facing_dir);
void boneym_turn_start(Actor * a);
void boneym_target_player(Actor * a);
void boneym_attack(Actor * a);
void boneym_animate(Sprite* sprite);

//Pointy
void spawn_pointy(int spawn_x,int spawn_y,u8 facing_dir);
void pointy_animate(Sprite* sprite);
void pointy_update(Actor * a);






