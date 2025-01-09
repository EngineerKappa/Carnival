u8 transition_scroll;
u8 transition_gm;
u8 transition_state;
void (*transition_callback)();
void transition_init();
void transition_start(void *callback);
void transition_animate_enter();
void transition_animate_exit();
void transition_animate();

void transition_title_start_game();
void transition_room_next();