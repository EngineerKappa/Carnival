enum {
    GAME_STATE_NORMAL = 0,
    GAME_STATE_MOVE,
    GAME_STATE_ATTACK,
    GAME_STATE_DEATH,
    GAME_STATE_GATE,
    GAME_STATE_GAMEOVER
};

u8 gate_speed;
u8 game_pixels_scrolled;

u8 floor_current;
u16 yorb_count;
u16 step_count;
bool update_hud;
#define PLAYER_HP_MAX 10
u8 player_hp;

void game_init();
void game_end();
void game_update();
void game_update_gate();
void game_draw_hud_text();
void game_draw_hud_labels();
void game_update_attack();
void game_update_death();
void game_update_move();
void menu_gameover_continue();
void transition_gameover_continue();
void menu_gameover_end_game();
void transition_gameover_end_game();