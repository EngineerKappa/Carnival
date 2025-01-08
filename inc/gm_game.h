enum {
    GAME_STATE_NORMAL = 0,
    GAME_STATE_MOVE,
};
u8 game_pixels_scrolled;
void game_init();
void game_end();
void game_update();