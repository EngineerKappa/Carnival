enum {
    GAME_STATE_NORMAL = 0,
    GAME_STATE_MOVE,
    GAME_STATE_GATE,
};
u8 gate_speed;
u8 game_pixels_scrolled;
void game_init();
void game_end();
void game_update();
void game_draw_hud_text();
void game_draw_hud_labels();
u8 floor_current;
u16 yorb_count;
u16 step_count;
bool update_hud;


void game_run_move();