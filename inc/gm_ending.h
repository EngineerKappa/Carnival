enum {
    ENDING_STATE_ENTER = 0,
    ENDING_STATE_TALLY,
};

void ending_init();
void ending_update();
void ending_end();

void transition_game_clear();