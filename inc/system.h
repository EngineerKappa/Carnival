
enum {
    GM_LOGO = 0,
    GM_TITLE,
    GM_GAME,
    GM_ENDING
};



void system_init();
void system_update();
void gm_change(u8 gm);
void gm_start(u8 gm);
void gm_end();
void (*func_update)();
u8 gm_current;
u16 gm_timer;
u8 gm_state;

