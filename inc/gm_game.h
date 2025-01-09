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
u8 level_current;
u16 yorb_count;
u16 step_count;


void game_run_move();