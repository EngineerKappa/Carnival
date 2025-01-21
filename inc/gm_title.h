enum {
    TITLE_STATE_NORMAL = 0,
    TITLE_STATE_CREDITS
};

void title_init();
void title_update();
void title_end();
void title_draw();
void menu_title_start_game();
void menu_title_credits();